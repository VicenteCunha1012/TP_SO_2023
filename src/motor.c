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
/*
    lockFile = open(lockFilename, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(lockFile == -1) {
        if(errno==EEXIST) {
            printf("Apenas pode correr uma instancia deste programa de cada vez\n");
            exit(EXIT_FAILURE);
        } else {
            perror("open");
            exit(EXIT_FAILURE);
        }
    }
    signal(SIGINT, sigint_handler);
*/

    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    createMap("level.txt",mapBuffer);  
    
    //
    
    Avatar users[MAX_USERS];
    int currentPlayers = 0;
	
    pid_t pid = fork();
    
    if(pid == 0) {
    	mkfifo("jogoUIFIFO", 0666);
    	int receiveAvatarFd = 0;
        while(currentPlayers < MAX_USERS) {	
        	getPlayers(users, &currentPlayers, receiveAvatarFd);
        }
        
        printf("sai do primeiro while\n");
/*
        for(int i=0;i<MAX_USERS;i++) {
            do {
                
                users[i].x = handleXAndY('x');
                users[i].y = handleXAndY('y');
            }while(charInStr(mapBuffer[users[i].x,users[i].y*2], avoydables));
        }
*/

        InitPayload toSend;
        initPayload(&toSend, users, mapBuffer);

        int fdtantos = open("engineFIFO", O_WRONLY);
        
        for(int i=0;i<MAX_USERS;i++) {
        	write(fdtantos, &toSend, sizeof(toSend));
        }

        int fdEngine[MAX_USERS];
        for(int i=0;i< MAX_USERS;i++) {
            char tempBuffer[50];
            sprintf(tempBuffer, "%s%d",toSend.PlayersID[i].nome, toSend.PlayersID[i].pid);
            mkfifo(tempBuffer, 0666);
            fdEngine[i]=open(tempBuffer, O_WRONLY);
            if(fdEngine[i]==-1) {printf("Erro no canudo");}
            
        }
        printf("162");

        

        close(receiveAvatarFd);
        
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


