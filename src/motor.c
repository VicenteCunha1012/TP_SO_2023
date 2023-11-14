#include "./helpers/algoritmos.h"
#include <errno.h>

int main(int argc, char **argv) {
	// Verifica se ja existe um motor a correr
#if 0
    if(checkRunningInstance(LOCK_FILENAME)) {
        printf("Ja existe uma instancia deste programa a correr nesta maquina.\n");
        exit(0);
    }
    
#endif
    // Prepara o sinal para limpar
    signal(SIGINT, sigint_handler);
#if 1
	
    int inscricao, minPlayers, duracao, decremento; // Variaveis de ambiente
    char mapBuffer[MAP_ROWS][MAP_COLUMNS]; 			// Mapa do jogo
    Avatar users[MAX_USERS];						// Array de utilizadores ("clientes")
    int currentPlayers = 0;							// Numero atual de utilizadores
    int receiveFd;
    pid_t pid;
    int playerFIFOs[MAX_USERS];
    
    /*
    // Recebe as variaveis de ambiente
    if(!getEnvs(&inscricao,&minPlayers,&duracao,&decremento)) {
        fprintf(stderr, "Ocorreu um erro a obter as variaveis de ambiente.\n");
        exit(0);
    }
    
    printf("Vars de ambiente: %d,%d,%d,%d\n",inscricao,minPlayers,duracao,decremento);
	*/
	// Cria e abre o pipe para leitura do motor
	openEnginePipe(&receiveFd);										
    
	// Le o mapa de um ficheiro e coloca em mapBuffer
    createMap("level.txt",mapBuffer);  
    
    if((pid = fork()) == 0) {   	
        while(currentPlayers < MAX_USERS) {	
        	getPlayers(users, &currentPlayers, receiveFd);
            printf("%d\n",users[currentPlayers].pid);
        }

        InitPayload toSend;
        initPayload(&toSend, users, mapBuffer);

        if(!sendInitPack(users, playerFIFOs, MAX_USERS,toSend)) {
            printf("Ocorreu um erro a enviar informacoes aos clientes.\n");fflush(stdout);//REM
            exit(0);
        }
        close(receiveFd);
        unlink(receiveFd);
        
    } else {
        char commandBuffer[COMMAND_BUFFERSIZE] = "";

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
#endif
    //Exemplo de como receber dados do bot
#if 1
	initBot(2, 2);
#endif
    return 0;
}


