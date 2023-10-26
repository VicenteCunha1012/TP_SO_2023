#include "helpersMotor.h"
#include "comandosMotor.h"
#include <stdio.h>

int main(int argc, char **argv) {
    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    Avatar users[MAX_USERS];
    
    int fd;
    Avatar tempAvatar;
    //mkfifo("~/Desktop/cenas/TP_SO_2023", 0666);
    fd = open("~/Desktop/cenas/TP_SO_2023/testfifo", O_RDONLY);
    read(fd, &tempAvatar, sizeof(Avatar));
    close(fd);
    printf("%d", tempAvatar.name);
    
    //char commandInput[COMMAND_BUFFERSIZE];
    //readCommand(commandInput, sizeof(commandInput));
    //handleCommand(commandInput);
    return 0;
}


