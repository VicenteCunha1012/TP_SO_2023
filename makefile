# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Linker flags (libraries)
LDLIBS = -lncurses

# Directories
SRC_DIR = src
HEADERS_DIR = src/headers

# Executables
BOT_EXEC = $(SRC_DIR)/bot
JOGOUI_EXEC = $(SRC_DIR)/jogoUI
MOTOR_EXEC = $(SRC_DIR)/motor

# Targets
all: $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)

$(BOT_EXEC): $(SRC_DIR)/bot.c
	$(CC) $(CFLAGS) $(LDLIBS) -o $(BOT_EXEC) $(SRC_DIR)/bot.c

$(JOGOUI_EXEC): $(SRC_DIR)/jogoUI.c $(SRC_DIR)/helpersJogoUI.c $(HEADERS_DIR)/helpersJogoUI.h
	$(CC) $(CFLAGS) -o $(JOGOUI_EXEC) $(SRC_DIR)/jogoUI.c $(SRC_DIR)/helpersJogoUI.c $(LIBRARY_PATH) $(LDLIBS)

$(MOTOR_EXEC): $(SRC_DIR)/motor.c $(SRC_DIR)/comandosMotor.c $(HEADERS_DIR)/comandosMotor.h $(SRC_DIR)/helpersMotor.c $(HEADERS_DIR)/helpersMotor.h
	$(CC) $(CFLAGS) $(LDLIBS) -o $(MOTOR_EXEC) $(SRC_DIR)/motor.c $(SRC_DIR)/comandosMotor.c $(SRC_DIR)/helpersMotor.c

clean:
	rm -f $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)

