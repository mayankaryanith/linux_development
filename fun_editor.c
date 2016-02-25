
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
    #define KEY_ESC '\033'
    bool overtype_mode=false;
    fpos_t pos;
    int main(int argc, char *argv[])
    {
    int c,x,y;
    FILE *fp;
    long lSize;
    long Fin;
    long position;
    char * buffer;
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
           //If the file exists read the file
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
                    refresh();
               }

           }
            while((c=getch())!= KEY_ESC)
	      {
          switch(c)
          {
            case KEY_F(1):
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
               break;
            case KEY_RIGHT:
               nocbreak();
			   noecho();
			   getyx(stdscr,y,x);
			   move(y,x+1);
			   cbreak();
		       refresh();
               fseek(fp,+1,SEEK_CUR);
               break;
            case KEY_EOS:
               nocbreak();
			   noecho();
			   getmaxyx(stdscr,y,x);
			   move(y,x);
               cbreak();
               refresh();
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
                printw("wordcount: %d \n",ftell(fp));
                refresh();
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
                        refresh();
                    }

                }
                break;
            case KEY_BACKSPACE:  //ftruncate the file by each character when back_space is pressed,fd is required beacause as ftruncate only works with open 
		        nocbreak();
		        noecho();
		        getyx(stdscr,y,x);
		        move(y,x-1);
		        delch();
		        cbreak();
                ftruncate(fileno(fp),ftell(fp)-1);
                getbegyx(stdscr,y,x);
                move(y+1,x);
                erase();
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
                        refresh();
                    }

                }
                refresh();
                break;
            default: // write to the file
                if(overtype_mode)  // overtype mode where you overwrite a character in existing location
                {
                    fseek(fp, 0L, SEEK_END);
                    Fin=ftell(fp);
                    fseek(fp, position, SEEK_SET);//current "position" in the file
                    lSize = Fin - position;//lenght from where you want to insert the chars and the eof
                    buffer = (char*) malloc(sizeof(char) * lSize);
                    fread(buffer, 1, lSize, fp);//fread (Fin - position) into buffer
                    fseek(fp, position, SEEK_SET);//fseek to position
                    fputc(c, fp);//write the character and it is a char pointer
                    fputs(buffer, fp);//fwrite the buffer
                }
                getyx(stdscr,y,x);
                addch(c);
                refresh();
                fputc(mvinch(y,x) & A_CHARTEXT,fp);
                move(y,x+1);
             }
	      }
		  refresh();
		  fclose(fp);
          fp=NULL;
          free(buffer);
          buffer=NULL;
		  endwin();
	}
        return 0;
    }


