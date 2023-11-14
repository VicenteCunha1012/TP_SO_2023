#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "estruturas.h"

int sendInitPack(Avatar users[],int* playersFifos[], int userCount, InitPayload toSend);
int validateJogoUICommand(char* args, Avatar users[], Avatar myAvatar, int playerCount);
int cleanInput(char* args, int size);
int validateMotorCommand(char* args);
int checkRunningInstance(char* filename);

int handleXAndY(char escolha);
int charInStr(char caracter, char string[]);
int isNameAvailable(char nome[],Avatar array[], int tamanho);

void initAvatar(Avatar *myAvatar, char *name);
void initPayload(InitPayload *toSend, Avatar *users, const char *map);

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]);
//void handleSignal(int signo, siginfo_t *info, void *context);

int getEnvs(int* inscricao, int* minPlayers, int* duracao, int* decremento);

extern char avoydables[];
extern int newPlayerIsPlaying;
extern const char *lockFilename;
extern const char *killMessages[];

typedef int (*CommandFunc)(char* args);

void flattenMap(char mapToFlatten[MAP_ROWS][MAP_COLUMNS],char flatMap[]);
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

void sigalarm_handler(int signum);
void sigint_handler(int signum);
void getPlayers(Avatar *users, int *currentPlayers, int receiveAvatarFd);

void initBot(int interval, int duration);
void makeOpenEnginePipe(int *receiveFd);

#endif
