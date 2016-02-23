
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
    #include<unistd.h>
    #define KEY_ESC '\033'
    bool overtype_mode=false;

    int main(int argc, char *argv[])
    {

    int c,x,y,size=1,seek=0,word_count,line_count;
    int *buff,*buff_beg,*buff_end,**buff_curr,*temp_buff;
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
                printw("The editor is up and running press ESC to exit.\n");
		        refresh();
                fp=fopen(argv[1],"ab+");
        if(!fp)
        {
            printw("Error opening file");
            refresh();
		    endwin();
            exit(1);
        }
        else
        {
           buff_beg=buff_end=NULL;
           buff=malloc(size*sizeof(int));
           buff_curr=buff;
           if(!buff)
            {
                printw(" Malloc memory allocation failed");
                refresh();
                exit(1);
            }
           //If the file exists read the file
           while((c=getc(fp))!=EOF)
           {
               if(c!='\n') //dont know why I added the check for new line character
                {
                    addch(c);
                    //temp buffer that holds the value read from the file
                    buff[size-1]=c;
                    buff_curr=&buff[size-1];
                    size++;
                    if(!(temp_buff=realloc(buff,size*sizeof(int))))
                    {
                        printw("Realloc cannot allocate the requested memory address");
                        refresh();
                        free(buff);
                    }
                    buff=temp_buff;
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
                fseek(fp,0,SEEK_SET);
		        cbreak();
		        refresh();
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
		       nocbreak();
		       noecho();
		       getyx(stdscr,y,x);
		       move(y,x-1);
               fseek(fp,-1,SEEK_CUR);
               buff_curr--;
		       cbreak();
		       refresh();
               break;
            case KEY_RIGHT:
               nocbreak();
			   noecho();
			   getyx(stdscr,y,x);
			   move(y,x+1);
               fseek(fp,1,SEEK_CUR);
               buff_curr++;
			   cbreak();
		       refresh();
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
            //Flush word_count and line_count everytime keynpage is pressed so as to get the current value
                word_count=0;
                line_count=0;
            //for word count and line count, start from the begining till the EOF and count
                fseek(fp,0,SEEK_SET);
                while((c=getc(fp)!=EOF))
                {
                    word_count++;
                    if(c=='\n')
                    line_count++;
                }
                erase();
                printw("wordcount: %d \t",word_count);
                printw("linecount: %d",line_count);
                refresh();
           //could have added a goto Label but it can be fatal and lead to memory corruption and hence clear the screen and add the content on the screen again
           //no need to update the buffers buff and size value though
                fseek(fp,0,SEEK_SET);
                getbegyx(stdscr,y,x);
                move(y+1,x);
                refresh();
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
            case KEY_BACKSPACE:  //This is done by overwriting the deleted character with a blank space
		        nocbreak();
		        noecho();
		        getyx(stdscr,y,x);
		        move(y,x-1);
		        delch();
                fseek(fp,-1,SEEK_CUR);
                fputc('\0',fp);
		        cbreak();
		        refresh();
            default: // write to the file
                if(!overtype_mode)  // overtype mode where you overwrite a character in existing location
                {
                    //temp buffer that holds the value of the text entered through the keyboard
                        buff[size-1]=c;
                        buff_curr=&buff[size-1];
                        size++;
                    if(!(temp_buff=realloc(buff,size*sizeof(int))))
                    {
                        printw("Realloc cannot allocate the requested memory address");
                        refresh();
                        free(buff);
                    }
                    buff=temp_buff;
                    //We change the file pointer to a position to the right to check if their is non \0 charater to the right
                    fseek(fp,1,SEEK_CUR);
                    if((c=fgetc(fp))!= EOF)
                    {
                        while(**buff_curr!='\0')
                        {
                            fputc(**buff_curr,fp);
                            buff_curr++;
                            seek++;
                            size++;
                    //Ncurses screen needs to be updated with dynamic shifting and printing of characters to the right
                            getyx(stdscr,y,x);
                            move(y,x+1);
                            addch(**buff_curr);
                        }
                    //Restore to current position and set seek to 0 and bring back the current buff to previous location movecursor location on the screen where it was assuming it fits in one line :(
                        buff_curr=buff_curr-seek;
                        fseek(fp,-seek,SEEK_CUR);
                        move(y,x-seek);
                        refresh();
                        seek=0;
                    }
                }
                getyx(stdscr,y,x);
                addch(c);
                fputc(mvinch(y,x-1) & A_CHARTEXT,fp);
                move(y,x+1);
             }
	      }
		  refresh();
		  fclose(fp);
          fp=NULL;
          free(buff);
          buff=buff_curr=NULL;
		  endwin();
	}
        return 0;
    }


