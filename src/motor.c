#include "./helpers/algoritmos.h"
#include <errno.h>


int RUNNING = 1;

void termina(int sig) {
	RUNNING = 0;
}





int main(int argc, char **argv) {
    if(checkRunningInstance(LOCK_FILENAME)) {
        printf("Ja existe uma instancia deste programa a correr nesta maquina.\n");
        exit(0);
    }
    
    signal(SIGINT,sigint_handler);
#if 1
    int inscricao, minPlayers, duracao, decremento; //vars do ambiente
    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    Avatar users[MAX_USERS];
    int currentPlayers = 0;
    
    
    if(!getEnvs(&inscricao,&minPlayers,&duracao,&decremento)) {
        printf("Ocorreu um erro a obter as variaveis de ambiente.\n");
        exit(0);
    }
    printf("Vars de ambiente: %d,%d,%d,%d\n",inscricao,minPlayers,duracao,decremento);

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
            printf("%d\n",users[currentPlayers].pid);
        }

        for(int i=0;i<MAX_USERS;i++) {
            printf("%s/%d\n",users[i].nome,users[i].pid);fflush(stdout);
        }
        
        printf("<motor filho> sai do primeiro while\n");fflush(stdout); //REM

        InitPayload toSend;
        initPayload(&toSend, users, mapBuffer);

        printf("<motor filho> payload construida\n"); fflush(stdout);//REM
        

        int playerFIFOs[5];

        

        if(!sendInitPack(users, playerFIFOs, MAX_USERS,toSend)) {
            printf("Ocorreu um erro a enviar informacoes aos clientes.\n");fflush(stdout);//REM
            exit(0);
        }

        
        
        // for(int i=0;i<MAX_USERS;++i) {
        //     printf("<motor filho> iteracao %d\n",i);fflush(stdout); //REM
        //     char nome[20];
        //     sprintf(nome, FIFO_CLIENTE, users[i].pid);
        //     printf("<motor filho> A tentar abrir %s\n",nome);fflush(stdout); //REM
        //     playerFIFOs[i] = open(nome, O_RDONLY);
        //     if(playerFIFOs[i]<0) {printf("erro");exit(0);}
        //     int nbytes = write(playerFIFOs[i],&toSend, sizeof(toSend));
        // }
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
#endif
    //Exemplo de como receber dados do bot
#if 1
	signal(SIGINT, termina);
    char frase[20];
    int pipe_fd[2];
    if(pipe(pipe_fd) == -1){
    	return 1;
    }
    pid_t pid2 = fork();
    int child = pid2 == 0;
    if(child) {
    	close(pipe_fd[0]); // Close read end of the pipe

        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);

        close(pipe_fd[1]); // Close the original write end of the pipe

        // Execute the bot program
        execlp("./bot", "./bot", "2", "2", NULL);

        // If execl fails
        perror("execl");
        exit(EXIT_FAILURE);

    } else {
    	close(pipe_fd[1]); // Close write end of the pipe

        char buffer[256];
        ssize_t bytesRead;

        while (RUNNING) {
            bytesRead = read(pipe_fd[0], buffer, sizeof(buffer) - 1);

            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                printf("Received: %s", buffer); // Store it in your variable or process as needed
                fflush(stdout);
            }

            //sleep(1);
        }

        close(pipe_fd[0]); // Close read end of the pipe

        // Wait for the child process to complete
        waitpid(pid2, NULL, 0);
        printf("A sair\n");
        fflush(stdout);
    }
    


#endif

    //char commandInput[COMMAND_BUFFERSIZE];
    //readCommand(commandInput, sizeof(commandInput));
    //handleCommand(commandInput);
    return 0;
}


