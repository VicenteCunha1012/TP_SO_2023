#include "./helpers/algoritmos.h"

void sigint_handler1(int signum) {
    char nome[20];
    sprintf(nome, FIFO_CLIENTE, getpid());
    unlink(nome);  
    printf("\n\ncliente fechado\n\n"); 
    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGINT, sigint_handler1);
    if(argc != 2 || strlen(argv[1]) > 20) {
        fprintf(stderr, "Erro: Sintaxe Invalida. Por favor introduza o seu nome (MAX 20 caracteres)\nExemplo: ./jogoUI \"John Doe\"\n");
        exit(0);
    }
    


    //
    
    Avatar myAvatar;
    initAvatar(&myAvatar, argv[1]);
    
    //
    
	int sendAvatarFd = open(FIFO_SERVIDOR, O_WRONLY);
    if(sendAvatarFd==-1) {
        printf("Ocorreu um erro aler o sendAvatarfd\n");
        exit(0);
    }
	if(write(sendAvatarFd, &myAvatar, sizeof(Avatar)) == -1) {
		perror("Erro a enviar Avatar ao motor");
		exit(EXIT_FAILURE);
	}
    printf("ja enviei");
    fflush(stdout);
	close(sendAvatarFd);

    char FIFOname[20];
    sprintf(FIFOname, FIFO_CLIENTE,getpid());
    printf("<cliente> antes de criar pipe com nome %s\n",FIFOname);fflush(stdout); //REM
    mkfifo(FIFOname , 0777);
	
    //
    
    InitPayload payload;
    
    fflush(stdout);
    int receivePayloadFd = open(FIFOname , O_RDONLY);
    if(read(receivePayloadFd, &payload, sizeof(payload)) == -1) {
    	perror("Erro a receber Payload do motor");
        fflush(stdout);
    }
    close(receivePayloadFd);
    
    //
    
    puts(payload.mapa);
    
    

    initScreen();
    
    WINDOW *topWindow = newwin(TOP_SCREEN_HEIGTH, TOP_SCREEN_WIDTH, 0, (COLS - TOP_SCREEN_WIDTH) / 2);
    WINDOW *bottomWindow= newwin(BOTTOM_SCREEN_HEIGTH, BOTTOM_SCREEN_WIDTH, (TOP_SCREEN_HEIGTH + PADDING), (COLS - BOTTOM_SCREEN_WIDTH) / 2);
    WINDOW *windows[N_WINDOWS] = {topWindow, bottomWindow};

    drawBorder(topWindow, bottomWindow);
    
    
    ungetch('.');
    getch();    
    
    wrefresh(topWindow);
    wrefresh(bottomWindow);

    printf("depois de refresh\n");

    for(int i=0;i<MAP_ROWS;i++) {
        for(int j=0;j<MAP_COLUMNS;j++) {
            if(payload.mapa[i][j]!='\n') {
                //mvprintw(topWindow, i+1, j+1, "%c",payload.mapa[i][j]);
                mvwaddch(topWindow, i+1, j+1, payload.mapa[i][j]);
                
            }
        }
    }


    int key;
    char command[STRING_SIZE];
    while ((key = getch())) {
        printf("entrou em while");
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
 		
 		// TODO ENVIAR AVATAR AO MOTOR
 		
        refreshAll(windows);
    }

    noraw();
    delwin(topWindow);
    delwin(bottomWindow);
    endwin();
    refresh();

    return 0;    

}

