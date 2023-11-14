#ifndef DEFINES_H
#define DEFINES_H

#include <unistd.h>
#include <ncurses.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define COMMAND_BUFFERSIZE 40
#define MAX_BOTS 10
#define MAX_ROCKS 50
#define MAX_LEVELS 3
#define MAX_ROLLINGSTONES 3

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

#define MAP_ROWS 16
#define MAP_COLUMNS 80
#define USER_NAME_SIZE 20
#define MAX_USERS 5
#define NAME_SENDER_FIFO "nameSender"

#define READ 0
#define WRITE 1

#define NAME_LENGTH 20
#define HEADER_SIZE 3
#define COMMAND_PACKET_SIZE 50

#define FIFO_CLIENTE "jogoUI%d"
#define FIFO_SERVIDOR "engine"
#define LOCK_FILENAME ".lock"


#endif
