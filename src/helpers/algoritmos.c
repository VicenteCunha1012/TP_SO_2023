#include "algoritmos.h"

int newPlayerIsPlaying = 1;
const char *lockFilename = ".lock";
int lockFile;
const char* killMessages[] = {
    "Abortaram-me :(",
    "Bateram-me as botas :(",
    "Esticaram-me o pernil :(",
    "Fiquei a servir de carpete :(",
    "Auch :(",
    "Espalmaram-me :(",
    "O Terminator apanhou-me :(",
    "Mataram-me :(",
};

int getEnvs(int* inscricao, int* minPlayers, int* duracao, int* decremento) {
    if(getenv("INSCRICAO")==NULL || getenv("NPLAYERS")==NULL || getenv("DURACAO")==NULL || getenv("DECREMENTO")==NULL) {
        return 0;
    }
    *inscricao = atoi(getenv("INSCRICAO"));
    printf("saquei um ");fflush(stdout);
    *minPlayers = atoi(getenv("NPLAYERS"));
    *duracao = atoi(getenv("DURACAO"));
    *decremento = atoi(getenv("DECREMENTO"));
    
    if(*inscricao < 0 || *minPlayers > 5 || *minPlayers < 0 || *duracao < 0 || *decremento < 0) {
        return 0;
    }
    return 1;
}


int isNameAvailable(char nome[], Avatar array[], int tamanho) {
    for(int i = 0; i < tamanho; ++i) {
        if(strcmp(nome, array[i].nome) == 0) {
            return 0;
        }
    }
    return 1;
}

int charInStr(char caracter, char string[]) {
    for(int i = 0; i < AVOYDABLES_SIZE && string[i] != '\0'; ++i) {
        if(caracter == string[i]) {
            return 1;
        }
    }
    return 0;
}

int handleXAndY(char escolha) {
    switch(escolha) {
        case 'x':
            return rand() % MAP_ROWS;
            break;
        case 'y':
            return rand() % MAP_COLUMNS / 2; //porque vai andar de 2 em 2 e o mapa e de 16x40
            break;
        default: 
            return 0;
            break;    
    }
}


void initPayload(InitPayload *toSend, Avatar *users, const char *map) {
	for(int i=0;i<MAX_USERS;i++) {
        printf("\n<initPayload> iteracao%d\n",i);
        strcpy(toSend->PlayersID[i].nome, users[i].nome);
        toSend->PlayersID[i].icone = users[i].icone;
        toSend->PlayersID[i].state = users[i].state;
        toSend->PlayersID[i].cor = users[i].cor;
        toSend->PlayersID[i].x = users[i].x;
        toSend->PlayersID[i].y = users[i].y;
        toSend->PlayersID[i].pid = users[i].pid;
        toSend->PlayersID[i].isPlaying = users[i].isPlaying;
    }
    strcpy(toSend->mapa, map);
}

void initAvatar(Avatar *myAvatar, char *name) {
	strcpy(myAvatar->nome, name);
    myAvatar->icone = name[0];
    myAvatar->x = 0;
    myAvatar->y = 0;
    myAvatar->isPlaying = 0;
    myAvatar->state = ' ';
    myAvatar->cor = ' ';
	myAvatar->pid = getpid();
    myAvatar->isVerified = 0;
}


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

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]) {
    int fd;
    fd = open(file, O_RDONLY);
    for(int i = 0; i < MAP_ROWS; ++i) {
        for(int j = 0; j < MAP_COLUMNS; ++j) {
            read(fd, &mapBuffer[i][j], sizeof(char));
        }  
    } 
}


Command commands[] = {
    {"kick", kickCommand},
    {"users", usersCommand},
    {"bots", botsCommand},
    {"bmov", bmovCommand},
    {"rbm", rbmCommand},
    {"begin", beginCommand},
    {"end", endCommand}    
};

//ISTO SAO COMANDOS DO JOGOUI

// int validatePlayers(char* args){
//     if(!strcmp(args,"")) {
//         return 1; //executa
//     } else {
//         return 0;
//     }
// }
// int validateMsg(char* args){
//     //msg joao ola joao tudo bem
//     //joao ola joao tudo bem
//     char arg1[20], arg2[20];
//     sscanf(args,"%s %[^\n]",arg1,arg2);
    
//     if(!strcmp(arg1,"") || !strcmp(arg2,"")) {
//         return 0; 
//     }
//     return 1; //executa
// }
// int validateExit(char* args) {
//     if(!strcmp(args,"")) {
//         return 1; //executa
//     } else {
//         return 0;
//     }
// }

int validateJogoUICommand(char* args, Avatar users[], Avatar myAvatar, int playerCount) {
    char firstToken[20], secondToLast[100]="";
    sscanf(args,"%s %[^\n]",firstToken,secondToLast);
    if(!strcmp(firstToken,"players")) {
        //UNICO exemplo *players
        if(!strcmp(secondToLast,"")) {
            return JOGOUI_PLAYERS_SUCCESS;
        } 
        return JOGOUI_WRONG_ARGS;
        
    }else if(!strcmp(firstToken,"msg")) {
        //exemplo *msg joao ola joao
        char nome[USER_NAME_SIZE], thirdToLast[100]="";
        sscanf(secondToLast,"%s %[^\n]",nome,thirdToLast);
        int nameFound=0;
        if(!strcmp(myAvatar.nome,nome)) {
            return JOGOUI_SELF_NAME;
        }
        for(int i=0;i<playerCount;i++) {
            if(!strcmp(users[i].nome, nome && strcmp(thirdToLast,""))) { // se o jogador existir, nao for o proprio, e a mensagem nao for ""
                return JOGOUI_MSG_SUCCESS;
            }
        }
    }else if(!strcmp(firstToken,"exit")) {
        //UNICO exemplo *exit
        if(!strcmp(secondToLast,"")) {
            return JOGOUI_EXIT_SUCCESS;
        }
        return JOGOUI_WRONG_ARGS;
    } else {
        return JOGOUI_COMMAND_NOT_FOUND;
    }
}

int cleanInput(char* args, int size) {
    if (args == NULL || size <= 0) {
        // Invalid arguments
        return -1;
    }

    int i, j;
    bool spaceDetected = false;
    int changesMade = 0;

    // Traverse the string from the end to remove trailing spaces
    for (i = size - 1; i >= 0; i--) {
        if (args[i] == ' ' || args[i] == '\t' || args[i] == '\n' || args[i] == '\r') {
            args[i] = '\0'; // Replace trailing space with null terminator
            changesMade++;
        } else {
            break; // Stop when a non-space character is encountered
        }
    }

    // Traverse the string to eliminate double spaces
    for (i = 0, j = 0; i < size; i++) {
        if (args[i] == ' ' || args[i] == '\t' || args[i] == '\n' || args[i] == '\r') {
            if (!spaceDetected) {
                args[j++] = args[i]; // Copy the first space
                spaceDetected = true;
            } else {
                changesMade++;
            }
        } else {
            args[j++] = args[i]; // Copy non-space character
            spaceDetected = false;
        }
    }

    // Null-terminate the cleaned string
    args[j] = '\0';

    return changesMade;
}

int 

void initPlayersPos(Avatar* users[], int playerCount,char mapa[][]) {
    for(int i=0;i<playerCount;i++) {
        if(users[i]->isPlaying) {
            int isInAvoydable = 0;
            do {
            users[i]->x = handleXAndY('x');
            users[i]->y = handleXAndY('y');
                for(int i=0;i<AVOYDABLES_SIZE;i++) {
                    if(mapa[users[i]->x][users[i]->y*2]==avoydables[i]) {
                        isInAvoydable = 1;
                    }
                }
            } while(!isInAvoydable); 
        }
    }
}

int validateMotorCommand(char* args) {
    char command[20], secondToLast[100];
    sscanf(args, "%s %[^\n]",command, secondToLast);
    if(!strcmp(command,"kick")) {

    }else if(!strcmp(command,"users")) {
        if(!strcmp(secondToLast,"")) {
            return MOTOR_USERS_SUCCESS;
        } return MOTOR_WRONG_ARGS;
    }else if(!strcmp(command,"bots")) {

    }else if(!strcmp(command,"bmov")) {

    }else if(!strcmp(command,"rbm")) {

    }else if(!strcmp(command,"begin")) {
        if(!strcmp(secondToLast,"")) {
            return MOTOR_BEGIN_SUCCESS;
        } return MOTOR_WRONG_ARGS;
    }else if(!strcmp(command,"end")) { 
        if(!strcmp(secondToLast,"")) {
            return MOTOR_END_SUCCESS;
        } return MOTOR_WRONG_ARGS;
    }else if(!strcmp(command,"test_bot")){
    
    }else {
       return MOTOR_COMMAND_NOT_FOUND;
    }
}


// Command jogoUICommands[] = {
//     {"players",validatePlayers},
//     {"msg"    ,validateMsg},
//     {"exit"   ,validateExit}
//  };

//*ISTO SAO COMANDOS DO JOGOUI
//ISTO SAO COMANDOS DO VROOM

void kickCommand(char* args) {
    printf("Kicking %s!\n", args);
}

void usersCommand(char* args) {
	printf("Kicking %s!\n", args);
}

void botsCommand(char* args) {
	printf("Kicking %s!\n", args);
}

void bmovCommand(char* args) {
    printf("Kicking %s!\n", args);
}

void rbmCommand(char* args) {
    printf("Kicking %s!\n", args);
}

void beginCommand(char* args) {
    printf("Kicking %s!\n", args); 
}

void endCommand(char* args) {
    printf("Kicking %s!\n", args);
}


// Command Handlers
void readCommand(char* command, size_t commandSize) {
    fgets(command, commandSize, stdin);
    command[strcspn(command, "\n")] = 0;
}

void handleCommand(char* input) {
    char command[50];
    char args[50];
    sscanf(input, "%s %[^\n]", command, args);

    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
        int commandFound = (strcmp(commands[i].commandName, command) == 0);
        if (commandFound) {
            commands[i].func(args);
            return;
        }
    }

    printf("Unknown command: %s\n", command);
}

void sigalarm_handler(int signum) {
    printf("\nespetaram-me: %d\n", signum);
    fflush(stdout);
    newPlayerIsPlaying=0;
}
int checkRunningInstance(char* filename) {
    int fd = open(filename, O_RDONLY | O_CREAT | O_EXCL, 0666);
    // lockFile = open(lockFilename, O_WRONLY | O_CREAT | O_EXCL, 0666);
    // if(lockFile == -1) {
    //     if(errno==EEXIST) {
    //         printf("Apenas pode correr uma instancia deste programa de cada vez\n");
    //         exit(EXIT_FAILURE);
    //     } else {
    //         perror("open");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    if(fd == -1) {
        if(errno==EEXIST) {
            return 1;
        }
    }
    return 0;

    
}

int sendInitPack(Avatar users[],int* playersFifos[], int userCount, InitPayload toSend) {
    for(int i=0;i<userCount;i++) {
        printf("<sendInitPack> iteracao %d\n",i);fflush(stdout); //REM
        char nome[20];
        sprintf(nome, FIFO_CLIENTE, users[i].pid);
        playersFifos[i] = open(nome, O_WRONLY);
        if(playersFifos[i]<0){printf("<sendInitPack> erro a abrir pipe de cliente %d",i);exit(0);}
        int nbytes = write(playersFifos[i],&toSend, sizeof(toSend));
        if(nbytes==0) return 0;
        printf("<sendInitPack> fim de iteracao %d\n",i);
    }
    printf("<sendInitPack> em principio todos os initpacks foram enviados\n");
    return 1;
}

void sigint_handler(int signum) {
    srand(time(NULL));
    
    int random = (rand() % 8) ;
    printf("\n%s\n", killMessages[random]);

    //close(lockFile);
    unlink(LOCK_FILENAME);
    exit(0);
}


void getPlayers(Avatar *users, int *currentPlayers, int receiveAvatarFd) {
	Avatar tempAvatar;
    int nBytes = read(receiveAvatarFd, &tempAvatar, sizeof(Avatar));
    if(nBytes == 0) {
    	return;
    } else if(nBytes == -1) {
    	perror("Erro ao receber Avatar do jogoUI");
    }
    if(isNameAvailable(tempAvatar.nome, users, *currentPlayers)) {
        printf("Nao existe, a criar\n");
        tempAvatar.isPlaying = 1; 
    } else {
        printf("\nerro ja existe com este nome :)\n");
        tempAvatar.isPlaying = 0;
    }
        users[*currentPlayers] = tempAvatar;
        (*currentPlayers)++;
}

int RUNNING = 1;

void termina(int sig) {
	RUNNING = 0;
}

void initBot(int interval, int duration) {
	char intervalBuffer[3];
	sprintf(intervalBuffer, "%d", interval);
	char durationBuffer[3];
	sprintf(durationBuffer, "%d", duration);
	signal(SIGINT, termina);
    char frase[20];
    int pipe_fd[2];
    if(pipe(pipe_fd) == -1){
    	return 1;
    }
    pid_t pid2 = fork();
    int child = pid2 == 0;
    if(child) {
    	close(pipe_fd[0]); 

        dup2(pipe_fd[1], STDOUT_FILENO);

        close(pipe_fd[1]);

        execlp("./bot", "./bot", intervalBuffer, durationBuffer, NULL);

        perror("execl");
        exit(EXIT_FAILURE);

    } else {
    	close(pipe_fd[1]);

        char buffer[256];
        ssize_t bytesRead;

        while (RUNNING) {
            bytesRead = read(pipe_fd[0], buffer, sizeof(buffer) - 1);

            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; 
                printf("Received: %s", buffer);
                fflush(stdout);
            }
        }

        close(pipe_fd[0]); 

        waitpid(pid2, NULL, 0);
        printf("A sair\n");
        fflush(stdout);
    }
}


void makeOpenEnginePipe(int *receiveFd) {
	mkfifo(FIFO_SERVIDOR, 0777);
    
    if((*receiveFd = open(FIFO_SERVIDOR, O_RDWR)) < 0) {
        printf("Ocorreu um erro a abrir o pipe");
        exit(0);
    }
}


