#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "estruturas.h"


int checkAvatarExistingNome(char nome[],Avatar array[], int tamanho);

void handleSignal(int signo, siginfo_t *info, void *context);
void initCommunication();
void sendMessage(pid_t receiver_pid, const char *message);

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]);
//void handleSignal(int signo, siginfo_t *info, void *context);

extern char avoydables[];

typedef void (*CommandFunc)(char* args);


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



// WINDOW FUNTIONS

void writeWindowLabel(char string[], int size, WINDOW* window);
void refreshAll(WINDOW* windows[]);
void drawBorder(WINDOW *topWindow, WINDOW *bottomWindow);
void initScreen();

// PLAYER FUNCTIONS

int isAvoydable(char avoydables[], WINDOW* window, int x, int y);
void placeAvatar(Avatar avatar, WINDOW* window);
void getCommandLine(WINDOW *bottomWindow, char *command);




#endif
