#include "./helpers/algoritmos.h"


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
/*
typedef struct {
    typedef struct {
        char nome[NAME_LENGTH];
        int x,y;
        char icone=nome[0];
    }avatar;
    pid_t PID;

}jogador;

typedef struct {
    char header[HEADER_SIZE];
    union dados{
        int x;
        char mensagem[COMMAND_PACKET_SIZE];
    }dados;
}packet;


*/

int newPlayerIsPlaying =1;

void sigusr1_handler(int signum) {
            printf("\nespetaram-me\n");
            fflush(stdout);
            newPlayerIsPlaying=0;
        }



int main(int argc, char **argv) {
    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    Avatar users[MAX_USERS];
    int currentPlayers = 0;
    //Exemplo de como receber estrutura do jogoUI
    
    int fd;

    mkfifo("jogoUIFIFO", 0666);
    mkfifo("engineFIFO", 0666);


    createMap("level.txt",mapBuffer);

    
    
	
    

    pid_t PID = fork();
    if(PID==0) {
        signal(SIGUSR1,sigusr1_handler);

        
        
        //puto
        while(currentPlayers < 1) {			// TODO: ESTA A LER 2 VEZES SEGUIDAS POR ALGUMA RAZAO
            Avatar tempAvatar;
            fd = open("jogoUIFIFO", O_RDONLY);
            int n = read(fd, &tempAvatar, sizeof(Avatar));
            if(n == 0) continue;
            if(checkAvatarExistingNome(tempAvatar.nome,users,currentPlayers)) {
                //mandar mensagem de erro
            }  else {
            tempAvatar.isPlaying = newPlayerIsPlaying;
            users[currentPlayers] = tempAvatar;
            //printf("\n%s,%d\n",tempAvatar.nome,tempAvatar.isPlaying);
            //fflush(stdout);
            currentPlayers++;
            

            }
        }
        close(fd);
        
    } else {
        char commandBuffer[COMMAND_BUFFERSIZE]="";

        while(strcmp(commandBuffer,"exit")) {
            
            readCommand(commandBuffer,sizeof(commandBuffer));
            if(!strcmp(commandBuffer,"begin")) {
                printf("\nenviado\n");
                fflush(stdout);
                kill(PID, SIGUSR1);
            }
            handleCommand(commandBuffer);

        }

    }
    int fd2 = open("engineFIFO",O_WRONLY);
    if(fd2==-1) {printf("erro no canudo");}
    char flatMap[MAP_ROWS * MAP_COLUMNS+1];
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLUMNS; j++) {
            flatMap[i * MAP_COLUMNS + j] = mapBuffer[i][j];
        }
    }
    flatMap[MAP_ROWS*(MAP_COLUMNS)] = '\0';
    if(write(fd2,flatMap,sizeof(char)*MAP_ROWS*MAP_COLUMNS+1)==-1) {
        printf("\nocorreu um erro\n");
    }
    printf("supostamente mandou%s",flatMap);
    
    
    
    
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


