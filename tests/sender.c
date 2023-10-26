#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char * argv[]) {
    int myPipeC = open("myPipeC", O_RDWR);
    int myPipeS = open("myPipeS",O_RDWR);
    pid_t cpid;
    char buf[255];
    char send[255];
    int n;
    int saveIn = dup(fileno(stdin));
    int saveOut = dup(fileno(stdout));
    while(1) {
        if ((cpid = fork()) == 0 ) {
            while(1){
                while((n=read(myPipeC,&buf,255)) > 0){
                    write(fileno(stdout), &buf, n*sizeof(char));
                }
            }
        } else {
            while(1){
                fgets(send, 255,stdin);
                int len = strlen(send);
                write(myPipeS,send,(len+1)*sizeof(char));
            }
        }
    }
    close(myPipeC);
    close(myPipeS);
    fflush(stdin);
    fflush(stdout);
    dup2(saveIn,fileno(stdin));
    dup2(saveOut, fileno(stdout));
    exit(EXIT_SUCCESS);
}
