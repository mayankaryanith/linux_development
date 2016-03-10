/*** How to compile the code ***/
/*
gcc -Wall -std=gnu99 -o esc_ex esc_ex.c -lncurses
*/

/*** List of keycodes for ncurses ***/
/*KEY_BREAK	Break key
KEY_DOWN	Arrow down
KEY_UP	Arrow up
KEY_LEFT	Arrow left
KEY_RIGHT	Arrow right
KEY_HOME	Home key
KEY_BACKSPACE	Backspace
KEY_F0	Function key zero
KEY_DL	Delete line
KEY_IL	Insert line
KEY_DC	Delete character
KEY_IC	Insert char or enter insert mode
KEY_EIC	Exit insert char mode
KEY_CLEAR	Clear screen
KEY_EOS	Clear to end of screen
KEY_EOL	Clear to end of line
KEY_SF	Scroll 1 line forward
KEY_SR	Scroll 1 line backward (reverse)
KEY_NPAGE	Next page
KEY_PPAGE	Previous page
KEY_STAB	Set tab
KEY_CTAB	Clear tab
KEY_CATAB	Clear all tabs
KEY_ENTER	Enter or send
KEY_SRESET	Soft (partial) reset
KEY_RESET	Reset or hard reset
KEY_PRINT	Print or copy
KEY_LL	Home down or bottom (lower left)
KEY_A1	Upper left of keypad
KEY_A3	Upper right of keypad
KEY_B2	Center of keypad
KEY_C1	Lower left of keypad
KEY_C3	Lower right of keypad
KEY_BTAB	Back tab key
KEY_BEG	Beg(inning) key
KEY_CANCEL	Cancel key
KEY_CLOSE	Close key
KEY_COMMAND	Cmd (command) key
KEY_COPY	Copy key
KEY_CREATE	Create key
KEY_END	End key
KEY_EXIT	Exit key
KEY_FIND	Find key
KEY_HELP	Help key
KEY_MARK	Mark key
KEY_MESSAGE	Message key*/

//Golden rule is to make the variable local as much as possible to use less SRAM

/*Parameter to know the global state of the editor*/
    #include<stdio.h>
    #include<ncurses.h>
    #include<stdlib.h>
    #include<signal.h>
    #include<string.h>
    #include<sys/types.h>
    #include<unistd.h>
    #include<assert.h>
    #define KEY_ESC '\033'
    bool overtype_mode=false;
    fpos_t pos;
    void read_file(FILE *);
    void write_file(FILE *,bool, int, int, int);
    void backspace(FILE *,char *, int, int);
    void read_file(FILE *fp)
    {
                int c,x,y;
                fseek(fp,0,SEEK_SET);
                while((c=getc(fp))!=EOF)
                {
                    if(c!='\n') //dont know why I added the check for new line character
                    {
                        addch(c);
                    }
                    else
                    {
                        getyx(stdscr,y,x);
                        move(y+1,0);
                    }

                }
                refresh();
    }
    void write_file(FILE *fp, bool overtype_mode,int c, int curr, int end)
    {
        long y,x;
        char * buffer=NULL;
        if(overtype_mode)  // Insert mode where you overwrite a character in existing location
        {
            buffer = (char*) malloc(sizeof(char) * (end+1-curr));
            if(buffer !=NULL)
            {
                 fseek(fp,curr, SEEK_SET);//fseek to position
                 if(end-curr-1>0)
                 {
                    fread(buffer, sizeof(char) , end-curr-1, fp);//fread (end - curr) into buffer
                    fputc(c, fp);//write the character and it is a char pointer
                    fputs(buffer, fp);//fwrite the buffer
                    /* Update the map according to the file offset */
                    free(buffer);
                    buffer=NULL;
                    erase();
                    read_file(fp);
		            getbegyx(stdscr,y,x);
                    move(y,x-end+curr);
                    refresh();
                 }
                    /* For potential bug when the curser is at the end of the file buffer allocation fails*/
                 else
                 {
                    free(buffer);
                    buffer=NULL;
                 }
            }
        }
		else
		{
            getyx(stdscr,y,x);
            addch(c);
            refresh();
            fputc(mvinch(y,x) & A_CHARTEXT,fp);
            move(y,x+1);
            fseek(fp,0,SEEK_CUR);
		}
    }
    void backspace(FILE *fp, char * filename, int curr, int end)
    {
				long lSize,y,x;
                char * buffer1=NULL, *buffer2=NULL;
        if(curr==end)
		{
			ftruncate(fileno(fp),ftell(fp)-1);
			erase();
			read_file(fp);
		}
		else
		{
                lSize = end - curr;//length from where you want to insert the chars and the eof
                buffer1 = (char*) malloc(sizeof(char) * (curr-1));
                buffer2 = (char*) malloc(sizeof(char) * lSize);
				if(buffer1!=NULL)
				{
				fseek(fp,0,SEEK_SET);
                fread(buffer1, sizeof(char), curr-1, fp);//Read from the beginning of the file till curr-1
				}
                if(buffer2!=NULL)
				{
                    fseek(fp, curr, SEEK_SET);//current "position" in the file
                    fread(buffer2,sizeof(char), lSize, fp);//fread (end - map) into buffer
				}
                fseek(fp,0,SEEK_SET);//move to begining of the file
                if(buffer1!=NULL)
				    fwrite(buffer1,sizeof(char),sizeof(buffer1),fp);//fwrite the buffer
                fseek(fp,curr-1,SEEK_SET);
                if(buffer2!=NULL)
				    fwrite(buffer2,sizeof(char),sizeof(buffer2),fp);//fwrite the buffer
                fseek(fp,0,SEEK_END);
                /*Fix for the meta characters at the end of the file*/
			    ftruncate(fileno(fp),ftell(fp)-1);
                erase();
                read_file(fp);
		        getbegyx(stdscr,y,x);
		        move(y,x+curr-1);
                refresh();
                free(buffer1);
                free(buffer2);
                buffer1=NULL;
                buffer2=NULL;
		}
		
    }
    int main(int argc, char *argv[])
    {
    int c,x,y;
    FILE *fp;
        // If file name is not given close the program
        if(argc!=2)
        {
            printf("file name missing \n");
            exit(1);
        }
        // Start curses mode
		if (initscr() == NULL)
		{
                	fprintf(stderr, "Could not initialize screen\n");
                	exit(EXIT_FAILURE);
    	    	}
    	    	keypad(stdscr, TRUE);       // raw mode ON
    	    	noecho();                   // echo mode OFF
		        cbreak();                   //Line buffering disabled, every character enabled
    	    	scrollok(stdscr, TRUE);     // scrolling mode ON
		        refresh();
                fp=fopen(argv[1],"r+");
        if(!fp)//if the file does not exist, create it
        {
            fp=fopen(argv[1],"w+");
            if(fp==NULL)
            {
            printw("Error opening file");
            refresh();
            exit(1);
		    endwin();
            }
        }
        else
        {
            long map;
            long end;
		    read_file(fp);
            /*map and end coincides when we read the file*/
            map=ftell(fp);
            end=ftell(fp);
            while((c=getch())!= KEY_ESC)
	      {
		    switch(c)
              	{
                case KEY_F(1):      //undo insert tab
                    break;
                case KEY_F(2):
                break;
                case KEY_F(3):
                    break;
                case KEY_F(4):
                    break;
                case KEY_F(5):
                break;
                case KEY_F(6):
                    break;
                case KEY_F(7):
                    break;
                case KEY_F(8):
                break;
                case KEY_F(9):
                    break;
                case KEY_F(10):
                    break;
                case KEY_EOS:
                fp=fopen(argv[1],"w+");
		        read_file(fp);
                break;
                case KEY_HOME://It will bring to the beginning of the file
		            nocbreak();
		            noecho();
		            getyx(stdscr,y,x);
		            move(y,0);
		            cbreak();
		            refresh();
                    fseek(fp,0,SEEK_SET);
                    break;
                case KEY_UP:
		        nocbreak();
		        noecho();
		        getyx(stdscr,y,x);
		        move(y-1,x);
		        cbreak();
		        refresh();
                break;
                case KEY_PPAGE:
                    break;
                case KEY_IC:
                   overtype_mode=true;
                    break;
                case KEY_EIC:
                    overtype_mode=false;
                    map=0;
                    break;
                case KEY_LEFT:
                refresh();
		           nocbreak();
		           noecho();
		           getyx(stdscr,y,x);
		           move(y,x-1);
		           cbreak();
		           refresh();
                   fseek(fp,-1,SEEK_CUR);
                   map=ftell(fp);
                   break;
                case KEY_RIGHT:
                   nocbreak();
		    	   noecho();
		    	   getyx(stdscr,y,x);
		    	   move(y,x+1);
		    	   cbreak();
		           refresh();
                   fseek(fp,+1,SEEK_CUR);
                   map=ftell(fp);
                  	break;
                case KEY_DOWN:
                    nocbreak();
 	            	noecho();
 		            getyx(stdscr,y,x);
 		            move(y+1,x);
                	cbreak();
                	refresh();
 		            break;
                case KEY_NPAGE:
                    getbegyx(stdscr,y,x);
                    move(y+1,x);
                    erase();
                    refresh();
                    read_file(fp);
                    erase();
                    printw("wordcount: %d \t",ftell(fp));
                    refresh();
                    break;
                case KEY_BACKSPACE:  //backspace function implemented instead
		    	    fseek(fp,0,SEEK_CUR);
                    map=ftell(fp);
		    	    fseek(fp,0,SEEK_END);
                    end=ftell(fp);
                    backspace(fp,argv[1],map,end);
		    	    fseek(fp,map-1,SEEK_SET);
                    break;
                default: // write to the file
		    	    fseek(fp,0,SEEK_CUR);
                    map=ftell(fp);
		    	    fseek(fp,0,SEEK_END);
                    end=ftell(fp);
                    write_file(fp,overtype_mode,c,map,end);
                }
	      }
		  refresh();
		  fclose(fp);
          fp=NULL;
		  endwin();
	}
        return 0;
    }
