#include "./helpers/algoritmos.h"

int main(int argc, char** argv) {
    if(argc != 2 || strlen(argv[1]) > 20) {
        fprintf(stderr, "Erro: Sintaxe Invalida. Por favor introduza o seu nome (MAX 20 caracteres)\nExemplo: ./jogoUI \"John Doe\"\n");
        exit(0);
    }
    
    Avatar myAvatar;
    strcpy(myAvatar.nome, argv[1]);
    myAvatar.icone = argv[1][0];
    myAvatar.x = 0;
    myAvatar.y = 0;
    myAvatar.isPlaying = 0;
	
	mkfifo("jogoUIFIFO", 0666);
	int fd = open("jogoUIFIFO", O_WRONLY);
	write(fd, &myAvatar, sizeof(Avatar));
	close(fd);
    //agora podemos abrir um pipe com tipo engine+ o nome do player
    //para lhe devolver informacao de se ta a jogar etc?
    //ja para n flr de que ainda temos de implementar o p2p chat
    //peer to peer tipo sem mexer c o motor
    //podemos ter um 
    //e que tal o jogoUI tmb ter um fork a receber a informacao do sjogadores e quando chegar a 4 acaba
	//recebemos de volta o isPlaying talvez?
    int responseFd = 0;
    
    responseFd = open("engineFIFO",O_RDONLY);
    /*if(read(responseFd,&myAvatar.isPlaying, sizeof(myAvatar.isPlaying))==-1) {
        //erro a receber isPlaying
    }*/
    if(read(responseFd,myAvatar.isPlaying, sizeof(myAvatar.isPlaying))==-1) {
        printf("Nao sei o que fazer, serei jogador ou espectador?\n");
    }


    char map[MAP_ROWS][(MAP_COLUMNS)];
    char tempMap[MAP_ROWS][MAP_COLUMNS];

   char receivedData[MAP_ROWS * (MAP_COLUMNS)];  // Replace with the actual size you expect
    ssize_t bytes_read = read(responseFd, receivedData, sizeof(receivedData));
    
    if (bytes_read == -1) {
        perror("Error reading from named pipe");
    } else {
        if (bytes_read != sizeof(receivedData)) {
            fprintf(stderr, "Incomplete data received\n");
        } else {
            // Reconstruct the 2D array
            for (int i = 0; i < MAP_ROWS; i++) {
                for (int j = 0; j < (MAP_COLUMNS); j++) {
                    map[i][j] = receivedData[i * (MAP_COLUMNS) + j];
                }
            }

            // Now mapBuffer contains the reconstructed 2D array
        }
    }
    map[MAP_ROWS][MAP_COLUMNS] = '\0';
    puts(map);
    


    
    initScreen();
    
    WINDOW *topWindow = newwin(TOP_SCREEN_HEIGTH, TOP_SCREEN_WIDTH, 0, (COLS - TOP_SCREEN_WIDTH) / 2);
    WINDOW *bottomWindow= newwin(BOTTOM_SCREEN_HEIGTH, BOTTOM_SCREEN_WIDTH, (TOP_SCREEN_HEIGTH + PADDING), (COLS - BOTTOM_SCREEN_WIDTH) / 2);
    WINDOW *windows[N_WINDOWS] = {topWindow, bottomWindow};

    drawBorder(topWindow, bottomWindow);
    
    
    ungetch('.');
    getch();    
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLUMNS; j++) {
            if(map[i][j]!='\n') {

            mvwprintw(topWindow, i+1, j+1, "%c", map[i][j]);
            }
        }
    }
    if(myAvatar.isPlaying==1) {
        mvprintw(bottomWindow, 1, 1,"%s","Sou jogador.");
    } else {
        mvprintw(bottomWindow, 1, 1,"%s","Sou espectador.");
    }
     
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
