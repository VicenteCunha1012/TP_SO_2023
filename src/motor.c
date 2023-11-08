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
    
    if(pid == 0) {//puto
    	
    	
        while(currentPlayers < MAX_USERS) {	
        	getPlayers(users, &currentPlayers, receiveFd);
        }
        
        printf("<motor filho> sai do primeiro while\n"); //REM

        InitPayload toSend;
        initPayload(&toSend, users, mapBuffer);

        printf("<motor filho> payload construida\n"); fflush(stdout);//REM
        

        int playerFIFOs[5];

        for(int i=0;i<MAX_USERS;++i) {
            printf("<motor filho> iteracao %d\n",i);fflush(stdout); //REM
            char nome[20];
            sprintf(nome, FIFO_CLIENTE, users[i].pid);
            printf("<motor filho> A tentar abrir %s\n",nome);fflush(stdout); //REM
            playerFIFOs[i] = open(nome, O_RDONLY);
            if(playerFIFOs[i]<0) {printf("erro");exit(0);}
            int nbytes = write(playerFIFOs[i],&toSend, sizeof(toSend));
        }
        printf("<motor filho> sai do for\n");fflush(stdout);

        close(receiveFd);
        unlink(receiveFd);
        
    } else { //pai
        char commandBuffer[COMMAND_BUFFERSIZE]="";

        while(strcmp(commandBuffer,"exit")) {
            if(!strcmp(commandBuffer,"exit")) {
                exit(0);
            }
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


