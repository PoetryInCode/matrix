#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>

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

    while(getch()==ERR) {
        int xPos = rand()%maxX;//get random x position for the stream
        if(cascade[xPos]==ERR) {//make sure the stream is set to active
            cascade[xPos]=0;
		}
        for(int i=0; i<maxX; i++) {//move the streams down
            if(cascade[i]!=ERR) {
                tail[i] = cascade[i]-tailLen[i];
                if(cascade[i]==maxY) {//check if the position is at the bottom of the screen
                    cascade[i]=ERR;//set the position to ERR if it is at the bottom
                } else {
					if(rand()%10==1) {//check if the stream will jump
						if(!(jmpTail[i]<=maxY)) {
							jmpTail[i]=cascade[i]-tailLen[i];
							cascade[i]=cascade[i]+rand()%8;
						}
					} else {
						cascade[i]++;//increase the position
					}
                }
            }
            if(tail[i]!=maxY) {//keep increasing the tail untill it is at the bottom
                tail[i]++;
            }
			if(jmpTail[i]==maxY) {
				jmpTail[i]=OK;
			} else {
				jmpTail[i]++;
			}
			mvaddch(jmpTail[i],i,' ');
            mvaddch(tail[i],i,' ');
            mvaddch(cascade[i],i,(rand()%57)+65);
        }
        refresh();
    }
    endwin();
    return 0;
}
