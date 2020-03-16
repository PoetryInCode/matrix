#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>

#define UP (1)
#define DOWN (2)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

int main(int argc, char **argv) {
    WINDOW term = *initscr();
    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    noecho();
    if(nodelay(&term,TRUE)==ERR) {
        puts("Error could not enter no-delay mode");
        return ERR;
    }
    int time;
    if(argc >= 3) {
        time=atoi(argv[2]);
    } else {
        time=20;
    }
    timeout(time);
	int maxX = COLS;
    int maxY = LINES;

    int initTailLen;
    if(argc >= 2) {
		initTailLen=atoi(argv[1]);
    } else {
        initTailLen=10;
    }

	int tailLen[maxX];

	for(int i=0; i<maxX; i++) {
		int num=rand()%2;
		if(num>1) {
			tailLen[i]=initTailLen+(rand()%(4%initTailLen));
		} else {
			if(num<=1) {
				tailLen[i]=initTailLen-(rand()%(8%initTailLen));
			}
		}
	}

    int cascade[maxX];
    int tail[maxX];
	int jmpTail[maxX];

    for(int i=0; i<maxX; i++)
        cascade[i]=ERR;

	int pause = 0;
	int dir = DOWN;
	int limit = maxY;
	int startPos = 0;
	int get = getch();
    while(get!='q') {
		if(get=='s') {
			pause=TRUE;
		}
        int xPos = rand()%maxX;//get random x position for the stream
        if(cascade[xPos]==ERR) {//make sure the stream is set to active
            cascade[xPos]=startPos;
		}
		//---------swich to weird format-----------
        for(int i=0; i<maxX; i++)
		{//move the streams up/down
			if(!pause)
			{
            	if(cascade[i]!=ERR)
				{
					if(dir==DOWN) {
						tail[i] = cascade[i]-tailLen[i];
					} else {
						tail[i] = cascade[i]+tailLen[i];
					}
                	if(cascade[i]==limit)
					{//check if the position is at the bottom of the screen
                    	cascade[i]=ERR;//set the position to ERR if it is at the bottom
                	}
					else
					{
						if(rand()%10==1)
						{//check if the stream will jump
							if(!(jmpTail[i]<=maxY))
							{
								if(dir==DOWN)
								{
									cascade[i]=cascade[i]+rand()%8;
									jmpTail[i]=cascade[i]-tailLen[i];
								}
								else
								{
									cascade[i]=cascade[i]-rand()%8;
									jmpTail[i]=cascade[i]+tailLen[i];
								}
							}
						}
						else
						{
							if(dir==DOWN)
							{
								cascade[i]++;//increase the position
							}
							else
							{
								cascade[i]--;
							}
						}
                	}
            	}
			}
			//-------Back to normal format-------
            if(tail[i]!=limit) {//keep iterating the tail intill it is at the limit
				if(dir==DOWN) {
					tail[i]++;
				} else {
					tail[i]--;
				}
            }
			if(jmpTail[i]==maxY || jmpTail==0) {
				jmpTail[i]=OK;
			} else {
				if(dir==DOWN) {
					jmpTail[i]++;
				} else {
					jmpTail[i]--;
				}
			}
			mvaddch(jmpTail[i],i,' ');
            mvaddch(tail[i],i,' ');
            mvaddch(cascade[i],i,(rand()%60)+65);
        }
        refresh();
		if(!pause) {
			timeout(time);
		} else {
			timeout(-1);
			get=getch();
			switch (get) {
				case 's':
					pause=!pause;
				break;
				case 'r':
					pause=FALSE;
					if(dir==DOWN) {
						dir=UP;
						limit=0;
						startPos=maxY;
					} else {
						dir=DOWN;
						limit=maxY;
						startPos=0;
					}
				break;
				case 'q':
					get='q';
					break;
			}
			timeout(time);
		}
		if(get!='q') {
			get=getch();
		}
    }
    endwin();
    return 0;
}
