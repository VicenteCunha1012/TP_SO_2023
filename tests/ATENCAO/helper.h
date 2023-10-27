#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define HEADER_SIZE 3
#define COMMAND_PACKET_SIZE 50
#define NAME_LENGTH 20

#define MAX_ENTITIES 5 //jogadores ou espectadores 
#define BUFFER_PIPE_SIZE 40
#define NAME_BUFFER_SIZE 20

#define PID_SIZE 15




typedef struct packet{
    char header[HEADER_SIZE];
    union dados {
        int x;
        char mensagem[COMMAND_PACKET_SIZE];
    };
}packet;

typedef struct avatar{
    char nome[NAME_LENGTH];
    int x,y;
    char icone;
}avatar;

typedef struct jogador{
    avatar avatar;
    pid_t PID;
}jogador;


#endif