#ifndef HELPERSMOTOR_H
#define HELPERSMOTOR_H

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdio.h>

#define MAP_ROWS 16
#define MAP_COLUMNS 80
#define USER_NAME_SIZE 20
#define MAX_USERS 5
#define NAME_SENDER_FIFO "~/TP_SO_2023/nameSender"


typedef struct {
    char name[20];
    int x;
    int y;
    int isPlaying;
    pid_t pid;
} Avatar;

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]);
void handleSignal(int signo, siginfo_t *info, void *context);

#endif