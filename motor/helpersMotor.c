#include "helpersMotor.h"

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]) {
    int fd;
    fd = open(file, O_RDONLY);
    for(int i = 0; i < MAP_ROWS; ++i) {
        for(int j = 0; j < MAP_COLUMNS; ++j) {
            read(fd, &mapBuffer[i][j], sizeof(char));
        }
    } 
}
