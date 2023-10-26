#include "helpersJogoUI.h"
char avoydables[4] = "x|-";


void writeWindowLabel(char string[], int size, WINDOW* window) {
    for(int i = 0; i < size; ++i) {
        mvwaddch(window, 0, i, string[i]);
    }
}

void placeAvatar(Avatar avatar, WINDOW* window) {
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

void drawBorder(WINDOW *topWindow, WINDOW *bottomWindow) {
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

