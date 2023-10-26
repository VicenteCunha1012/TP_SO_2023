#include "./headers/comandosMotor.h"

// Command Lookup Table
Command commands[] = {
    {"kick", kickCommand},
    {"users", usersCommand},
    {"bots", botsCommand},
    {"bmov", bmovCommand},
    {"rbm", rbmCommand},
    {"begin", beginCommand},
    {"end", endCommand}
};

void kickCommand(char* args) {
    printf("Kicking %s!\n", args);
    /*
        INFORM USER IT'S BEING KICKED
        END IT'S PROGRAM
    */
}

void usersCommand(char* args) {
    /*
        LIST CURRENT PLAYERS
    */
}

void botsCommand(char* args) {
    /*
        LIST CURRENT BOTS
    */
}

void bmovCommand(char* args) {
    /*
        ADD NEW ROLLING STONE
        SET STARTING POSITION IN AN EMPTY CELL
        MOVE STONE EVERY SECOND TO AN EMPTY CELL
    */
}

void rbmCommand(char* args) {
    /*
        DELETES ROLLING STONE
        IF THERE'S MORE THAN ONE, DELETE FIRST CREATED
    */
}

void beginCommand(char* args) {
    /*
        BEGINS GAME EVEN IF THERE'S NO PLAYERS
    */
}

void endCommand(char* args) {
    /*
        ENDS THE GAME
        TERMINATE "MOTOR"
        NOTIFY PLAYERS THE GAME ENDED AND TERMINATE THEM
        TERMINATE BOTS
        FREE RESOURCES
    */
}


// Command Handlers
void readCommand(char* command, size_t commandSize) {
    fgets(command, commandSize, stdin);
    command[strcspn(command, "\n")] = 0;
}

void handleCommand(char* input) {
    char command[50];
    char args[50];
    sscanf(input, "%s %[^\n]", command, args);

    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
        int commandFound = (strcmp(commands[i].commandName, command) == 0);
        if (commandFound) {
            commands[i].func(args);
            return;
        }
    }

    printf("Unknown command: %s\n", command);
}

