#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define STRINGSIZE 20
#define topHeight 16
#define topWidth 40*2
#define bottomHeight 10
#define bottomWidth  30*2
#define bottomDistFromTop 1
//16x40(*2)

void writeWindowLabel(char string[], int tamanho, WINDOW* window) {
    for(int i=0;i<tamanho;i++) {
        mvwaddch(window,0,i,string[i]);
    }
}

typedef struct avatar {
    char nome[20];
    int x;
    int y;
    char icone;

}avatar;


int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    

    // Create top and bottom windows
    int totalLines = LINES,totalColumns = COLS;
    
    WINDOW *top_win = newwin(topHeight, topWidth, 0, (COLS-topWidth)/2);
    WINDOW *bottom_win = newwin(bottomHeight, bottomWidth, (topHeight+bottomDistFromTop), (COLS-bottomWidth)/2);

    wborder(top_win, '|', '|', '-', '-', '+', '+', '+', '+');

    wborder(bottom_win, '|', '|', '-', '-', '+', '+', '+', '+');
    
    writeWindowLabel("+Bottom",strlen("+Bottom"),bottom_win);
    writeWindowLabel("+-Janela de Jogo",strlen("+-Janela de Jogo"),top_win);
    

    getch();
    wrefresh(top_win);
    wrefresh(bottom_win);

    int ch;
    int downCount=0,upCount=0;
    int upX=0,upY=0;
    int currCarr=-1;
    char command[STRINGSIZE];
    
    while (ch = getch()) {
        switch(ch) {
            case(KEY_UP):
                mvwprintw(top_win, 1, 1, "Pressed%d",++upCount);

                wrefresh(top_win);
                break;
            case (KEY_DOWN):
                mvwprintw(bottom_win, 1, 1, "Pressed%d",++downCount);
                
                wrefresh(bottom_win);
                break;
            case ('\n'):
                if(!strcmp(command,"clear")) {
                    memset(command,0,STRINGSIZE);
                    currCarr=-1;
                    wrefresh(bottom_win);
                } else {

                command[currCarr+1] = '\0';
                mvwprintw(bottom_win,3,0,   "                                  ");
                mvwprintw(bottom_win, 3, 0, "[Introduziu]: %s",command);
                wrefresh(bottom_win);
                }
                break;
            case (KEY_BACKSPACE):
                if(currCarr<0) {

                } else {

                command[currCarr]='\0';
                currCarr--;
                mvwprintw(bottom_win,2,1,"                                        ");
                mvwprintw(bottom_win,2,1,"%s",command);
                }
                wrefresh(bottom_win);

            default:
                
                if(isalnum(ch) || (ch==32)) {
                    
                    if(currCarr>=STRINGSIZE-1) {
                        
                    } else {
                        mvwprintw(bottom_win,2,++currCarr+1,"%c",ch);
                        command[currCarr] = ch;
                    }
                    wrefresh(bottom_win);
                }


        }
    }

    noraw();
    delwin(top_win);
    delwin(bottom_win);
    endwin();
    refresh();

    return 0;
}