#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include "defines.h"

typedef void (*CommandFunc)(char* args);

typedef struct {
    char nome[20];
    int x;
    int y;
    char icone;
    pid_t pid;
    int isPlaying;
} Avatar;


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