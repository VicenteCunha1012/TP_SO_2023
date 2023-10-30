#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "helper.h"


int main(int argc, char** argv) {
    //protocolo
    //argv[1] = nome
    //argv[2] = x
    //argv[3] = y
    //argv[4] obviamente n e o pid
    //deixo o error checking para o tomas

    int ui_to_engine_fd_global = open("ui_to_engine", O_WRONLY);
    int engine_to_ui_fd_global = open("engine_to_ui", O_RDONLY);
    
    /*
    char jogadorStringified[BUFFER_PIPE_SIZE];
    strcpy(jogadorStringified,argv[1]);
    strcat(jogadorStringified,argv[2]);
    strcat(jogadorStringified,argv[3]);
    char PIDBUFFER[PID_SIZE];
    sprintf(PIDBUFFER,"%d",getpid());
    strcat(jogadorStringified,PIDBUFFER);
	*/
	
	Avatar av;
	strcpy(av.nome, argv[1]);
	av.x = atoi(argv[2]);
	av.y = atoi(argv[3]);
	
	
    const char* filename=".lock";
    pid_t motorPID;
    int lockFile = open(filename, O_RDONLY,0666);
    printf("Resultado do read: %d",read(lockFile,&motorPID,sizeof(motorPID)));
    printf("\nSuposto PID: %d\n",motorPID);
    kill(motorPID, SIGUSR1);
    //kill(target_pid, SIGUSR1
    write(ui_to_engine_fd_global,jogadorStringified,sizeof(jogadorStringified));
    
    printf("\nsupostamente ta mandado\n");  


}




