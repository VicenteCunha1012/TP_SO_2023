#ifndef COMANDOSMOTOR_H
#define COMANDOSMOTOR_H

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define COMMANDBUFFERSIZE 40
#define MAXUSERS 5
#define MAXBOTS 10
#define MAXROCKS 50
#define MAXLEVELS 3
#define MAXROLLINGSTONES 3

typedef void (*CommandFunc)(char* args);

typedef struct {
    char* commandName;
    CommandFunc func;
} Command;

extern Command commands[]; // Command Lookup Table

// Commands
void kickCommand(char* args);
void usersCommand(char* args);
void botsCommand(char* args);
void bmovCommand(char* args);
void rbmCommand(char* args);
void beginCommand(char* args);
void endCommand(char* args);

// Command Handlers
void readCommand(char* command, size_t commandSize);
void handleCommand(char* input);


#endif //COMANDOSMOTOR_H