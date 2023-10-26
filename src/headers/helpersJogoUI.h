#ifndef HELPERSJOGOUI_H
#define HELPERSJOGOUI_H

#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define STRING_SIZE 20
#define TOP_SCREEN_HEIGTH 18
#define TOP_SCREEN_WIDTH 41*2
#define BOTTOM_SCREEN_HEIGTH 10
#define BOTTOM_SCREEN_WIDTH  30*2
#define COMMAND_LINE_X 5
#define COMMAND_LINE_Y BOTTOM_SCREEN_HEIGTH - 2
#define PADDING 1
#define AVOYDABLES_SIZE 4 
#define N_WINDOWS 2
#define KEY_SPACEBAR 32

typedef struct {
    char nome[20];
    int x;
    int y;
    char icone;
    pid_t pid;
} Avatar;

extern char avoydables[];

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
