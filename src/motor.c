#include "./helpers/algoritmos.h"

int main(int argc, char **argv) {
    if(checkRunningInstance(LOCK_FILENAME)) {
        fprintf(stderr, "Ja existe uma instancia deste programa a correr nesta maquina.\n");
        exit(0);
    }
    
    int inscricao, minPlayers, duracao, decremento; 		// Variáveis de Ambiente
    char mapBuffer[MAP_ROWS][MAP_COLUMNS], levelName[7];	// Buffer para o mapa de cada nível
    Avatar users[MAX_USERS];								// Array de utilizadores ("clientes")
    int currentPlayers = 0, currentLevel = 1;				// Número de utilizadores e nível atual 
    int receiveFd;											// Descritor de ficheiros para receber informação
    pid_t pid; 		
    int playerFifos[MAX_USERS];
    						
	signal(SIGINT,sigint_handler);  
	  
  	// Recebe as variáveis de Ambiente
    if(!getEnvs(&inscricao,&minPlayers,&duracao,&decremento)) {
        fprintf(stderr, "Ocorreu um erro a obter as variaveis de ambiente.\n");
        exit(0);
    }

	// Faz e abre o Named Pipe para receber informação
    makeOpenEnginePipe(&receiveFd);
    
    if(!(pid = fork())) {
        while(currentPlayers < MAX_USERS) {	
        	getPlayers(users, &currentPlayers, receiveFd);
        } //Recebe as informações dos jogadores através do pipe FIFO_SERVIDOR

		sprintf(levelName, "level%d.txt", currentLevel);
    	createMap(levelName, mapBuffer);  //Lê o ficheiro com nome levelName e importa o mapa lá escrito
    	
        InitPayload toSend;
        initPayload(&toSend, users, mapBuffer); //Prepara o pacote primario que envia uma vez a todos os clientes
                                                //com informacoes do mapa e de todos os users

		if(!sendInitPack(users, playerFifos, MAX_USERS, toSend)) {
			printf("Ocorreu um erro a enviar informacao aos clientes\n");
			exit(0);
		} //envia o pacote

        

        
		
        close(receiveFd);
        unlink(receiveFd);
        
    } else { //pai
        char commandBuffer[COMMAND_BUFFERSIZE]="";

        while(strcmp(commandBuffer,"exit")) {
            if(!strcmp(commandBuffer,"exit")) {
                close(receiveFd);
                unlink(FIFO_SERVIDOR);
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

	initBot(2, 2);

    return 0;
}
