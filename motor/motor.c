#include "helpersMotor.h"
#include "comandosMotor.h"

int main(int argc, char **argv) {
    char mapBuffer[MAP_ROWS][MAP_COLUMNS];
    Avatar users[MAX_USERS];



    char commandInput[COMMAND_BUFFERSIZE];
    readCommand(commandInput, sizeof(commandInput));
    handleCommand(commandInput);
    return 0;
}


