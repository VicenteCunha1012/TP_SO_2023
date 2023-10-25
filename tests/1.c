#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define STRING_SIZE 20
#define TOP_SCREEN_HEIGTH 16
#define TOP_SCREEN_WIDTH 40*2
#define BOTTOM_SCREEN_HEIGTH 10
#define BOTTOM_SCREEN_WIDTH  30*2
#define COMMAND_LINE_X 5
#define COMMAND_LINE_Y BOTTOM_SCREEN_HEIGTH - 2
#define PADDING 1
#define AVOYDABLES_SIZE 4 
#define N_WINDOWS 2
#define KEY_SPACEBAR 32
//16x40(*2)

void writeWindowLabel(char string[], int size, WINDOW* window) {
    for(int i = 0; i < size; ++i) {
        mvwaddch(window, 0, i, string[i]);
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

void drawScreen(WINDOW *topWindow, WINDOW *bottomWindow) {
    wborder(topWindow, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(bottomWindow, '|', '|', '-', '-', '+', '+', '+', '+');
}

void initScreen() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void getCommandLine(WINDOW *bottomWindow, char *command) {
    echo();
    curs_set(2);
    wmove(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X);
    wgetnstr(bottomWindow, command, 19);
    noecho();
    curs_set(0);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Erro: Sintaxe Invalida. Por favor introduza o seu nome\nExemplo: ./jogoUI \"John Doe\"\n");
        exit(0);
    }


    initScreen();

    Avatar avatar1 = {"nome1", 10, 10, '1'};

    char avoydables[4] = "x|-"; //exemplo 
    avoydables[3] = '\0';
    
    WINDOW *topWindow = newwin(TOP_SCREEN_HEIGTH, TOP_SCREEN_WIDTH, 0, (COLS - TOP_SCREEN_WIDTH) / 2);
    WINDOW *bottomWindow= newwin(BOTTOM_SCREEN_HEIGTH, BOTTOM_SCREEN_WIDTH, (TOP_SCREEN_HEIGTH + PADDING), (COLS - BOTTOM_SCREEN_WIDTH) / 2);
    WINDOW* windows[N_WINDOWS] = {topWindow, bottomWindow};

    drawScreen(topWindow, bottomWindow);
    mvwprintw(topWindow, 2, 2, "%c", 'x');
    
    getch();    
    wrefresh(topWindow);
    wrefresh(bottomWindow);

    int key;
    char command[STRING_SIZE];
    while (key = getch()) {
        switch(key) {
        case(KEY_UP):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x, avatar1.y - 1)){
                avatar1.y--;
            }
            posicionarAvatar(avatar1, topWindow);
            break;

        case (KEY_DOWN):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x, avatar1.y + 1)) {
                avatar1.y++;
            }
            posicionarAvatar(avatar1, topWindow);
            break;

        case (KEY_LEFT):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x - 1, avatar1.y)) {
                avatar1.x--;
            }
            posicionarAvatar(avatar1, topWindow);
            break;

        case (KEY_RIGHT):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x + 1, avatar1.y)) {
                avatar1.x++;
            }
            posicionarAvatar(avatar1, topWindow);
            break; 

        case(KEY_SPACEBAR):
            mvwprintw(bottomWindow, COMMAND_LINE_Y, 1, "%s" ,"-->");
            getCommandLine(bottomWindow, command);
            wmove(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X);
            mvwprintw(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X, "                                        ");
            mvwprintw(bottomWindow, BOTTOM_SCREEN_HEIGTH - 3, 1, "[Introduziu]: %s",command);
            break;
        }
 
        refreshAll(windows);
    }

    noraw();
    delwin(topWindow);
    delwin(bottomWindow);
    endwin();
    refresh();

    return 0;
}
