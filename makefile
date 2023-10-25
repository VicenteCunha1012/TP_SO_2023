# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Linker flags (libraries)
LDLIBS = -lncurses

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
	$(CC) $(CFLAGS) $(LDLIBS) -o $(BOT_EXEC) $(BOT_DIR)/bot.c

$(JOGOUI_EXEC): $(JOGOUI_DIR)/jogoUI.c $(JOGOUI_DIR)/helpersJogoUI.c $(JOGOUI_DIR)/helpersJogoUI.h
	$(CC) $(CFLAGS) -o $(JOGOUI_EXEC) $(JOGOUI_DIR)/jogoUI.c $(JOGOUI_DIR)/helpersJogoUI.c $(LIBRARY_PATH) $(LDLIBS)


$(MOTOR_EXEC): $(MOTOR_DIR)/motor.c $(MOTOR_DIR)/comandosMotor.c $(MOTOR_DIR)/comandosMotor.h $(MOTOR_DIR)/helpersMotor.c $(MOTOR_DIR)/helpersMotor.h
	$(CC) $(CFLAGS) $(LDLIBS) -o $(MOTOR_EXEC) $(MOTOR_DIR)/motor.c $(MOTOR_DIR)/comandosMotor.c $(MOTOR_DIR)/helpersMotor.c

clean:
	rm -f $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)
