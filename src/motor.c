#include "./helpers/algoritmos.h"
#include <errno.h>


/*
	Precisa de:
		-> Array dos jogadores
		-> Estrutura dos jogadores: (nome, pos_ atual, pid, )
		-> Estrutura packet com union 
	
	Mensagens entre jogadores:
		-> jogoUI envia nome ao motor
		-> Motor envia pid ao jogoUI
		-> jogoUI manda um sinal para o pid recebido
		-> o pid recebido prepara-se para receber mensagem
		-> jogoUI envia mensagem
	
		
	Update dos players:
		-> Apos avaliacao das teclas
		-> Enviar estado ao motor (x, y, etc)
		-> Motor envia estado aos players	
*/


int main(int argc, char **argv) {
    int inscricao, minPlayers, duracao, decremento; //vars do ambiente
    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    Avatar users[MAX_USERS];
    int currentPlayers = 0;

    // if(!getEnvs(&inscricao,&minPlayers,&duracao,&decremento)) {
    //     printf("Ocorreu um erro a obter as variaveis de ambiente.\n");
    //     exit(0);
    // }

    mkfifo(FIFO_SERVIDOR, 0777);
    int receiveFd = open(FIFO_SERVIDOR, O_RDWR);
    if(receiveFd < 0) {
        printf("Ocorreu um erro a abrir o pipe");
        exit(0);
    }

    createMap("level.txt",mapBuffer);  
    
    
    pid_t pid = fork();
    
    if(pid == 0) {
    	
    	
        while(currentPlayers < MAX_USERS) {	
        	getPlayers(users, &currentPlayers, receiveFd);
        }
        
        printf("sai do primeiro while\n");

        InitPayload toSend;
        initPayload(&toSend, users, mapBuffer);

        for(int i=0;i<MAX_USERS;++i) {
            char nome[20];
            sprintf(nome, FIFO_CLIENTE, users[i].pid);
            printf("A tentar abrir %s",nome);
            fflush(stdout);
            int tempFd = open(nome, O_RDONLY);
            if(tempFd<0) {printf("erro");exit(0);}
            printf("A tentar escrever\n");
            fflush(stdout);
            int nbytes = write(tempFd,&toSend, sizeof(toSend));
        }

        close(receiveFd);
        unlink(receiveFd);
        
    } else { //pai
        char commandBuffer[COMMAND_BUFFERSIZE]="";

        while(strcmp(commandBuffer,"exit")) {
            
            readCommand(commandBuffer,sizeof(commandBuffer));
            if(!strcmp(commandBuffer,"begin")) {
                printf("\nenviado\n");
                fflush(stdout);
                kill(pid, SIGALRM);
            }
            handleCommand(commandBuffer);

        }

    }

    //Exemplo de como receber dados do bot
	/*
    char frase[20];
    int fd[2];
    if(pipe(fd) == -1){
    	return 1;
    }
    pid_t pid = fork();
    int child = pid == 0;
    if(child) {
    	close(WRITE);
    	dup(fd[WRITE]);
    	close(fd[WRITE]);
    	close(fd[READ]);
    	
    	execl("bot", "bot", NULL);
    	exit(0);

    } else {
    	close(fd[WRITE]);
    	read(fd[READ], frase, sizeof(frase));
    	close(fd[READ]);
    	
    	wait(NULL);
    }
    
    printf("%s", frase);
    */

    //char commandInput[COMMAND_BUFFERSIZE];
    //readCommand(commandInput, sizeof(commandInput));
    //handleCommand(commandInput);
    return 0;
}


