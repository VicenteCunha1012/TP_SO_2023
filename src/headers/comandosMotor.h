#ifndef COMANDOSMOTOR_H
#define COMANDOSMOTOR_H

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define COMMAND_BUFFERSIZE 40
#define MAX_BOTS 10
#define MAX_ROCKS 50
#define MAX_LEVELS 3
#define MAX_ROLLINGSTONES 3

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
