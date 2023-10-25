#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define STRING_SIZE 20
#define TOP_SCREEN_HEIGTH 16
#define TOP_SCREEN_WIDTH 40*2
#define BOTTOM_SCREEN_HEIGTH 10
#define BOTTOM_SCREEN_WIDTH  30*2
#define PADDING 1
#define AVOYDABLES_SIZE 4 
#define N_WINDOWS 2
//16x40(*2)

void writeWindowLabel(char string[], int tamanho, WINDOW* window) {
    for(int i=0;i<tamanho;i++) {
        mvwaddch(window,0,i,string[i]);
    }
}

typedef struct {
    char nome[20];
    int x;
    int y;
    char icone;

} Avatar;

void posicionarAvatar(Avatar avatar, WINDOW* window) {
    mvwprintw(window, avatar.y, avatar.x, "%c", avatar.icone);
}

int isAvoydable(char avoydables[], WINDOW* window, int x, int y) {
    char tempchar = mvwinch(window, y, x) & A_CHARTEXT;
    for(int i = 0; i < AVOYDABLES_SIZE; ++i) {
        if(tempchar == avoydables[i]) {
            return 0;
        }
    }
    return 1;
}

void refreshAll(WINDOW* windows[]) {
    for(int i = 0; i < N_WINDOWS; i++) {
        wrefresh(windows[i]);
    }
}

void initScreen() {

}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    Avatar avatar1;
    strcpy(avatar1.nome,"nome1");
    avatar1.x = 10;
    avatar1.y = 10;
    avatar1.icone = '1'; 

    int terminalEnabled = 0;

    char avoydables[4] = "x|-"; //exemplo 
    avoydables[3] = '\0';

    // Create top and bottom windows
    int totalLines = LINES,totalColumns = COLS;
    
    WINDOW *top_win = newwin(TOP_SCREEN_HEIGTH, TOP_SCREEN_WIDTH, 0, (COLS-TOP_SCREEN_WIDTH)/2);
    WINDOW *bottom_win = newwin(BOTTOM_SCREEN_HEIGTH, BOTTOM_SCREEN_WIDTH, (TOP_SCREEN_HEIGTH+PADDING), (COLS-BOTTOM_SCREEN_WIDTH)/2);
    WINDOW* windows[N_WINDOWS] = {top_win,bottom_win};

    wborder(top_win, '|', '|', '-', '-', '+', '+', '+', '+');

    wborder(bottom_win, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(top_win,2,2,"%c",'x');
    
    writeWindowLabel("+Bottom",strlen("+Bottom"),bottom_win);
    writeWindowLabel("+-Janela de Jogo",strlen("+-Janela de Jogo"),top_win);
    

    getch();
    wrefresh(top_win);
    wrefresh(bottom_win);

    int ch;
    int downCount=0,upCount=0;
    int upX=0,upY=0;
    int currCarr=-1;
    char command[STRING_SIZE];
    while (ch = getch()) {
        if(!terminalEnabled) {
            switch(ch) {
            case(KEY_UP):
                mvwprintw(top_win, avatar1.y, avatar1.x, "%c",' ');
                if(isAvoydable(avoydables,top_win,avatar1.x,avatar1.y-1)){
                    avatar1.y--;
                }
                posicionarAvatar(avatar1, top_win);
                break;

            case (KEY_DOWN):
                mvwprintw(top_win, avatar1.y, avatar1.x, "%c",' ');
                if(isAvoydable(avoydables,top_win,avatar1.x,avatar1.y+1)) {
                    avatar1.y++;
                }
                posicionarAvatar(avatar1, top_win);
                break;

            case (KEY_LEFT):
                mvwprintw(top_win, avatar1.y, avatar1.x, "%c",' ');
                if(isAvoydable(avoydables,top_win,avatar1.x-1,avatar1.y)) {
                    avatar1.x--;

                }
                posicionarAvatar(avatar1, top_win);
                break;

            case (KEY_RIGHT):
                mvwprintw(top_win, avatar1.y, avatar1.x, "%c",' ');
                if(isAvoydable(avoydables,top_win,avatar1.x+1,avatar1.y)) {
                    avatar1.x++;
                }
                posicionarAvatar(avatar1, top_win);
                break; 

            case(32):
                terminalEnabled=!terminalEnabled;
                break;

            }
        } else {
            /*switch(ch) {
            case ('\n'):
                command[currCarr+1] = '\0';
                mvwprintw(bottom_win,BOTTOM_SCREEN_HEIGTH-3,1,   "                                  ");
                mvwprintw(bottom_win, BOTTOM_SCREEN_HEIGTH-3, 1, "[Introduziu]: %s",command);
                terminalEnabled=!terminalEnabled;
                break;

            case (KEY_BACKSPACE):
                if(currCarr >= 0) {
                    command[currCarr]='\0';
                    currCarr--;
                    mvwprintw(bottom_win,BOTTOM_SCREEN_HEIGTH-2,4,"                                        ");
                    mvwprintw(bottom_win,BOTTOM_SCREEN_HEIGTH-2,4,"%s",command);
                }
                
            default:
            */    if(currCarr < STRING_SIZE-1) {
                    mvwprintw(bottom_win,BOTTOM_SCREEN_HEIGTH-2,1,"%s","-->");
                    echo();
                    wmove(bottom_win, BOTTOM_SCREEN_HEIGTH - 2, 5);
                    wgetstr(bottom_win, command);
                    noecho();
                    wmove(bottom_win, BOTTOM_SCREEN_HEIGTH - 2, 5);
                    mvwprintw(bottom_win,BOTTOM_SCREEN_HEIGTH-2,4,"                                        ");
                    mvwprintw(bottom_win, BOTTOM_SCREEN_HEIGTH-3, 1, "[Introduziu]: %s",command);
                    terminalEnabled = 0;
                    //mvwprintw(bottom_win,BOTTOM_SCREEN_HEIGTH-2,++currCarr+4,"%c",ch);
                    //command[currCarr] = ch;
                }
                //break;
            //}
        }
        
        refreshAll(windows);
    }

    noraw();
    delwin(top_win);
    delwin(bottom_win);
    endwin();
    refresh();

    return 0;
}
