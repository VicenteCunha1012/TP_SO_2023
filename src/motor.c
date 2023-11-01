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


int newPlayerIsPlaying =1;

void sigalarm_handler(int signum) {
            printf("\nespetaram-me\n");
            fflush(stdout);
            newPlayerIsPlaying=0;
        }


void sigint_handler(int signum) {
    srand(time(NULL));
    
    int random = (rand() % 8) ;
    printf("\n%s\n",killMessages[random]);
    // Add cleanup code here if needed
    close(lockFile);
    unlink(lockFilename);
    exit(0);
}

int main(int argc, char **argv) {
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
        while(currentPlayers < MAX_USERS) {			// TODO: ESTA A LER 2 VEZES SEGUIDAS POR ALGUMA RAZAO
            fd = open("jogoUIFIFO", O_RDONLY);
            Avatar tempAvatar;
            int nBytes = read(fd, &tempAvatar, sizeof(Avatar));
            if(nBytes == 0) continue;
            if(!checkAvatarExistingNome(tempAvatar.nome,users,currentPlayers)) {
                printf("Nao existe, a criar\n");
                tempAvatar.isPlaying = 1; //n e preciso o outro
            } else {
                printf("\nerro ja existe com este nome :)\n");
                tempAvatar.isPlaying = 0;
            }
                users[currentPlayers] = tempAvatar;
                currentPlayers++;
            
        }printf("sai do primeiro while\n");

        for(int i=0;i<MAX_USERS;i++) {
            do {
                
                users[i].x = handleXAndY('x');
                users[i].y = handleXAndY('y');
            }while(charInStr(mapBuffer[users[i].x,users[i].y*2], avoydables, AVOYDABLES_SIZE));
        }
        
        InitPayload toSend;
        for(int i=0;i<MAX_USERS;i++) {
            strcpy(toSend.PlayersID[i].nome, users[i].nome);
            toSend.PlayersID[i].icone = users[i].icone;
            toSend.PlayersID[i].state = users[i].state;
            toSend.PlayersID[i].cor = users[i].cor;
            toSend.PlayersID[i].x = users[i].x;
            toSend.PlayersID[i].y = users[i].y;
            toSend.PlayersID[i].pid = users[i].pid;
            toSend.PlayersID[i].pid = users[i].isPlaying;
        }
        char mapa[MAP_ROWS * MAP_COLUMNS];
        //flattenMap(mapBuffer,mapa );
        strcpy(toSend.mapa, mapBuffer);
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

        

        close(fd);
        
    } else { //pai
        char commandBuffer[COMMAND_BUFFERSIZE]="";

        while(strcmp(commandBuffer,"exit")) {
            
            readCommand(commandBuffer,sizeof(commandBuffer));
            if(!strcmp(commandBuffer,"begin")) {
                printf("\nenviado\n");
                fflush(stdout);
                kill(PID, SIGALRM);
            }
            handleCommand(commandBuffer);

        }

    }
    /*int fd2 = open("engineFIFO",O_WRONLY);
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
    printf("supostamente mandou\n");*/
    

    
    
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


