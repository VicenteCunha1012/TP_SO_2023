#include "helpersJogoUI.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Erro: Sintaxe Invalida. Por favor introduza o seu nome\nExemplo: ./jogoUI \"John Doe\"\n");
        exit(0);
    }


    initScreen();

    Avatar avatar1 = {"nome1", 10, 10, '1'};

    avoydables[3] = '\0';
    
    WINDOW *topWindow = newwin(TOP_SCREEN_HEIGTH, TOP_SCREEN_WIDTH, 0, (COLS - TOP_SCREEN_WIDTH) / 2);
    WINDOW *bottomWindow= newwin(BOTTOM_SCREEN_HEIGTH, BOTTOM_SCREEN_WIDTH, (TOP_SCREEN_HEIGTH + PADDING), (COLS - BOTTOM_SCREEN_WIDTH) / 2);
    WINDOW* windows[N_WINDOWS] = {topWindow, bottomWindow};

    drawBorder(topWindow, bottomWindow);
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
            placeAvatar(avatar1, topWindow);
            break;

        case (KEY_DOWN):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x, avatar1.y + 1)) {
                avatar1.y++;
            }
            placeAvatar(avatar1, topWindow);
            break;

        case (KEY_LEFT):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x - 1, avatar1.y)) {
                avatar1.x--;
            }
            placeAvatar(avatar1, topWindow);
            break;

        case (KEY_RIGHT):
            mvwprintw(topWindow, avatar1.y, avatar1.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, avatar1.x + 1, avatar1.y)) {
                avatar1.x++;
            }
            placeAvatar(avatar1, topWindow);
            break; 

        case(KEY_SPACEBAR):                     //TODO: PROBABLYE NEED TO SEND THE MESSAGE TO THE "MOTOR" TO DISPLAY IN EVERY SCREEN
            mvwprintw(bottomWindow, COMMAND_LINE_Y, 1, "%s" ,"-->");
            getCommandLine(bottomWindow, command);                  //TODO: AFTER THIS, SEND MESSAGE TO THE "MOTOR", MAYBE MAKE A PACKET STRUCT WITH THE NAME AND MESSAGE
            
            //wmove(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X);
            //mvwprintw(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X, "                                        ");
            //mvwprintw(bottomWindow, BOTTOM_SCREEN_HEIGTH - 3, 1, "[%s]: %s", avatar1.nome, command);
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