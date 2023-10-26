#include "./headers/helpersMotor.h"
#include "./headers/comandosMotor.h"
#include <stdio.h>

#define READ 0
#define WRITE 1

/*
	Precisa:
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
    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    Avatar users[MAX_USERS];
    int currentPlayers = 0;
    //Exemplo de como receber estrutura do jogoUI
    int fd;
    while(currentPlayers < 5) {			// TODO: ESTA A LER 2 VEZES SEGUIDAS POR ALGUMA RAZAO
		Avatar tempAvatar;
		mkfifo("jogoUIFIFO", 0666);
		fd = open("jogoUIFIFO", O_RDONLY);
		read(fd, &tempAvatar, sizeof(Avatar));
		close(fd);
		users[currentPlayers] = tempAvatar;
		printf("%s", tempAvatar.pid);
		fflush(stdout);
		currentPlayers++;
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


