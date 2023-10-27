#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "helper.h"



const char* lockFilename = ".lock";
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
    "I'll be back",
};

const jogador latestPlayer;

int ui_to_engine_fd_global, engine_to_ui_fd_global;

jogador jogadores[MAX_ENTITIES];
int entityCount=0;

void sigint_handler(int signum) {
    srand(time(NULL));

    int random = rand() % 8;
    printf("\n%s\n",killMessages[random]);
    close(lockFile);
    unlink(lockFilename);
    exit(0);
}

jogador parseJogador(char message[]) {
    //isto nao verifica erros
    //protocolo, exemplo
    //meunome.10.04.123456\0
    int i=0;
    char namebuffer[NAME_BUFFER_SIZE];
    while(message[i]!='.') { //receber nome
        namebuffer[i] = message[i];
        i++;
    }
    namebuffer[i]='\0';
    int x;
    char xBuffer[3];
    while(message[i]!='.') {
        xBuffer[i]=message[i];
        i++;
    }
    x = atoi(xBuffer);
    int y;
    char yBuffer[3];
    while(message[i]!='.') {
        yBuffer[i]=message[i];
        i++;
    }
    y=atoi(yBuffer);
    pid_t PID; //um int basicamente
    char PIDBuffer[PID_SIZE];
    while(message[i]!='\0') {
        PIDBuffer[i]=message[i];
        i++;
    } 
    PID = atoi(PIDBuffer);

    jogador aRetornar = {{namebuffer,x,y},PID};
    return aRetornar;
    
}

void sigusr1_handler(int signum) {
    printf("Recebi signal usr1");
    char message[BUFFER_PIPE_SIZE];
    if(read(ui_to_engine_fd_global,&message,sizeof(message))==-1) {
        printf("\nOcorreu um erro a receber os dados de uma entidade, o motor vai tentar reiniciar\n");
        close(lockFile);
        unlink(lockFilename);
        close(ui_to_engine_fd_global);
        close(engine_to_ui_fd_global);
        execl("./motor","./motor",NULL);
    }
    printf("Recebi: %s",message);
    jogadores[entityCount] = parseJogador(message);
    entityCount++;

    printf("Jogadores\n");
    printf("%s:%d:%d:%d",jogadores[0].avatar.nome, jogadores[0].avatar.x,jogadores[0].avatar.y,jogadores[0].PID);
    
}



int main() {
    printf("\niniciodomain\n");
    lockFile = open(lockFilename, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(lockFile == -1) {
        if(errno==EEXIST) {
            printf("\nApenas pode correr uma instancia deste programa de cada vez\n");
            exit(EXIT_FAILURE);
        } else {
            printf("\nOcorreu um erro\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("cheguei a antes dos signals");
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);

    if (mkfifo("ui_to_engine", 0666) == -1) {
        perror("mkfifo");
        printf("Error creating named pipe: %s\n", strerror(errno));
    } else {
        printf("Named pipe 'ui_to_engine' created successfully.\n");
        ui_to_engine_fd_global = open("ui_to_engine", O_RDONLY);
}

    if (mkfifo("engine_to_ui", 0666) == -1) {
        perror("mkfifo");
        printf("Error creating named pipe: %s\n", strerror(errno));
    } else {
        printf("Named pipe 'engine_to_ui' created successfully.\n");
        engine_to_ui_fd_global = open("engine_to_ui", O_RDONLY);
    }

    
    printf("\ncheguei a antes dos opens dos pipes\n");
    printf("\ncheguei a linha 114\n");
    pid_t pid = getpid();
    write(lockFile, &pid,sizeof(pid));
    printf("\nescrevi %d\n",pid);
    while(1) {
        sleep(1);
        
    }

    
    

    //programa aqui

    

    close(lockFile);
    unlink(lockFilename);

    return 0;
}