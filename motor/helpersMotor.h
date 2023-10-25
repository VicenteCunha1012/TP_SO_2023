#ifndef HELPERSMOTOR_H
#define HELPERSMOTOR_H

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAP_ROWS 16
#define MAP_COLUMNS 80
#define USER_NAME_SIZE 20
#define MAX_USERS 5

typedef struct {
    char name[20];
    int x;
    int y;
    int isPlaying;
} Avatar;

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]);


#endif