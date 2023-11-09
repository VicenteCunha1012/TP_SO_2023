#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include "defines.h"

typedef int (*CommandFunc)(char* args);

typedef struct {
    char nome[USER_NAME_SIZE];
    char icone;
    char state;
    char cor;
    int x, y;
    pid_t pid;
    int isPlaying;
} Avatar;

typedef struct {
    char nome[USER_NAME_SIZE];
    pid_t pid;
} PlayerID;

typedef struct {
    char mapa[MAP_ROWS][MAP_COLUMNS];
    Avatar PlayersID[MAX_USERS];
} InitPayload;

typedef struct {
    char* commandName;
    CommandFunc func;
} Command;

typedef struct {
    char header[3];
    union message_t {
        int x;
        char comando [COMMAND_PACKET_SIZE];
    } message_t;
} Packet;


#endif
