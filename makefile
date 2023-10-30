# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Linker flags (libraries)
LDLIBS = -lncurses

# Directories
SRC_DIR = src
HELPERS_DIR = src/helpers

# Executables
BOT_EXEC = $(SRC_DIR)/bot
JOGOUI_EXEC = $(SRC_DIR)/jogoUI
MOTOR_EXEC = $(SRC_DIR)/motor

# Targets
all: $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)

$(BOT_EXEC): $(SRC_DIR)/bot.c
	$(CC) $(CFLAGS)  -o $(BOT_EXEC) $(SRC_DIR)/bot.c $(LDLIBS)

$(JOGOUI_EXEC): $(SRC_DIR)/jogoUI.c $(HELPERS_DIR)/algoritmos.c $(HELPERS_DIR)/algoritmos.h
	$(CC) $(CFLAGS) -o $(JOGOUI_EXEC) $(SRC_DIR)/jogoUI.c $(HELPERS_DIR)/algoritmos.c $(LIBRARY_PATH) $(LDLIBS)

$(MOTOR_EXEC): $(SRC_DIR)/motor.c $(HELPERS_DIR)/algoritmos.c $(HELPERS_DIR)/algoritmos.h
	$(CC) $(CFLAGS)  -o $(MOTOR_EXEC) $(SRC_DIR)/motor.c $(HELPERS_DIR)/algoritmos.c $(LDLIBS)

clean:
	rm -f $(BOT_EXEC) $(JOGOUI_EXEC) $(MOTOR_EXEC)

