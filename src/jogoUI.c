#include "helpersJogoUI.h"

int main(int argc, char** argv) {
    if(argc != 2 || strlen(argv[1]) > 20) {
        fprintf(stderr, "Erro: Sintaxe Invalida. Por favor introduza o seu nome (MAX 20 caracteres)\nExemplo: ./jogoUI \"John Doe\"\n");
        exit(0);
    }

    char *name = argv[1];
    
    Avatar myAvatar;
    strcpy(myAvatar.nome, argv[1]);
    myAvatar.icone = argv[1][0];
    myAvatar.x = 0;
    myAvatar.y = 0;
	
	mkfifo("/tmp/jogoUIFIFO", 0666);
	int fd = open("~/Desktop/cenas/TP_SO_2023/testfifo", O_WRONLY);
	write(fd, &myAvatar, sizeof(Avatar));
	close(fd);
	
    initScreen();
    
    WINDOW *topWindow = newwin(TOP_SCREEN_HEIGTH, TOP_SCREEN_WIDTH, 0, (COLS - TOP_SCREEN_WIDTH) / 2);
    WINDOW *bottomWindow= newwin(BOTTOM_SCREEN_HEIGTH, BOTTOM_SCREEN_WIDTH, (TOP_SCREEN_HEIGTH + PADDING), (COLS - BOTTOM_SCREEN_WIDTH) / 2);
    WINDOW *windows[N_WINDOWS] = {topWindow, bottomWindow};

    drawBorder(topWindow, bottomWindow);
    mvwprintw(topWindow, 2, 2, "%c", 'x');
    
    ungetch('.');
    getch();    
    placeAvatar(myAvatar, topWindow);
    wrefresh(topWindow);
    wrefresh(bottomWindow);

    int key;
    char command[STRING_SIZE];
    while ((key = getch())) {
        switch(key) {
        case(KEY_UP):
            mvwprintw(topWindow, myAvatar.y, myAvatar.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, myAvatar.x, myAvatar.y - 1)){
                myAvatar.y--;
            }
            placeAvatar(myAvatar, topWindow);
            break;

        case (KEY_DOWN):
            mvwprintw(topWindow, myAvatar.y, myAvatar.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, myAvatar.x, myAvatar.y + 1)) {
                myAvatar.y++;
            }
            placeAvatar(myAvatar, topWindow);
            break;

        case (KEY_LEFT):
            mvwprintw(topWindow, myAvatar.y, myAvatar.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, myAvatar.x - 1, myAvatar.y)) {
                myAvatar.x--;
            }
            placeAvatar(myAvatar, topWindow);
            break;

        case (KEY_RIGHT):
            mvwprintw(topWindow, myAvatar.y, myAvatar.x, "%c", ' ');
            if(isAvoydable(avoydables, topWindow, myAvatar.x + 1, myAvatar.y)) {
                myAvatar.x++;
            }
            placeAvatar(myAvatar, topWindow);
            break; 

        case(KEY_SPACEBAR):                     //TODO: PROBABLYE NEED TO SEND THE MESSAGE TO THE "MOTOR" TO DISPLAY IN EVERY SCREEN
            mvwprintw(bottomWindow, COMMAND_LINE_Y, 1, "%s" ,"-->");
            getCommandLine(bottomWindow, command);                  //TODO: AFTER THIS, SEND MESSAGE TO THE "MOTOR", MAYBE MAKE A PACKET STRUCT WITH THE NAME AND MESSAGE
            
            wmove(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X);
            mvwprintw(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X, "                                        ");
            mvwprintw(bottomWindow, BOTTOM_SCREEN_HEIGTH - 3, 1, "[%s]: %s", myAvatar.nome, command);
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
