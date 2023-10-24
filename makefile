# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -lncurses

# Directories
BOT_DIR = bot
JOGOUI_DIR = jogoUI
MOTOR_DIR = motor

# Executables
BOT_EXEC = $(BOT_DIR)/bot
JOGOUI_EXEC = $(JOGOUI_DIR)/jogoUI
MOTOR_EXEC = $(MOTOR_DIR)/motor

# Targets
all: $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)

$(BOT_EXEC): $(BOT_DIR)/bot.c
	$(CC) $(CFLAGS) -o $(BOT_EXEC) $(BOT_DIR)/bot.c

$(JOGOUI_EXEC): $(JOGOUI_DIR)/jogoUI.c
	$(CC) $(CFLAGS) -o $(JOGOUI_EXEC) $(JOGOUI_DIR)/jogoUI.c

$(MOTOR_EXEC): $(MOTOR_DIR)/motor.c $(MOTOR_DIR)/comandosMotor.c $(MOTOR_DIR)/comandosMotor.h
	$(CC) $(CFLAGS) -o $(MOTOR_EXEC) $(MOTOR_DIR)/motor.c $(MOTOR_DIR)/comandosMotor.c

clean:
	rm -f $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)
