#include <stdio.h>
#include "comandosMotor.h"


// char* createMap(char* file) -> return map

int main(int argc, char **argv) {
    char commandInput[COMMANDBUFFERSIZE];
    readCommand(commandInput, sizeof(commandInput));
    handleCommand(commandInput);
    return 0;
}


