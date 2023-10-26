#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>


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

void sigint_handler(int signum) {
    srand(time(NULL));
    
    int random = (rand() % 8) ;
    printf("\n%s\n",killMessages[random]);
    // Add cleanup code here if needed
    close(lockFile);
    unlink(lockFilename);
    exit(0);
}

int main() {
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
    int numero;
    printf("Diz me um numero sla");
    scanf("%d",&numero);


    close(lockFile);
    unlink(lockFilename);

    return 0;
}