#include "algoritmos.h"


int checkAvatarExistingNome(char nome[],Avatar array[], int tamanho) {
    for(int i=0;i<tamanho;i++) {
        if(!strcmp(nome,array[i].nome)) {
            return 1;
        }
    }
    return 0;
}

char avoydables[4] = "x|-";


void writeWindowLabel(char string[], int size, WINDOW* window) {
    for(int i = 0; i < size; ++i) {
        mvwaddch(window, 0, i, string[i]);
    }
}

void placeAvatar(Avatar avatar, WINDOW* window) {
    mvwprintw(window, avatar.y, avatar.x, "%c", avatar.icone);
}

int isAvoydable(char avoydables[], WINDOW* window, int x, int y) {
    char tempchar = mvwinch(window, y, x) & A_CHARTEXT;
    for(int i = 0; i < AVOYDABLES_SIZE; ++i) {
        if(tempchar == avoydables[i]) {
            return 0;
        }
    }
    return 1;
}

void refreshAll(WINDOW* windows[]) {
    for(int i = 0; i < N_WINDOWS; i++) {
        wrefresh(windows[i]);
    }
}

void drawBorder(WINDOW *topWindow, WINDOW *bottomWindow) {
    wborder(topWindow, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(bottomWindow, '|', '|', '-', '-', '+', '+', '+', '+');
}

void initScreen() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void getCommandLine(WINDOW *bottomWindow, char *command) {
    echo();
    curs_set(2);
    wmove(bottomWindow, COMMAND_LINE_Y, COMMAND_LINE_X);
    wgetnstr(bottomWindow, command, 19);
    noecho();
    curs_set(0);
}

void createMap(char* file, char mapBuffer[MAP_ROWS][MAP_COLUMNS]) {
    int fd;
    fd = open(file, O_RDONLY);
    for(int i = 0; i < MAP_ROWS; ++i) {
        for(int j = 0; j < MAP_COLUMNS; ++j) {
            read(fd, &mapBuffer[i][j], sizeof(char));
        }
    } 
}

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


static int pipe_fd;

void handleSignal(int signo, siginfo_t *info, void *context) {
	// Read the message pointer from the signal info
	const char *message = (const char*)info->si_value.sival_ptr;
	
	// Write the message to the pipe
	write(pipe_fd, message, strlen(message) + 1);
}

void initCommunication() {
	// Create a named pipe
	mkfifo("commPipe", 0666);
	
	//Open the pipe for writing
	pipe_fd = open("commPipe", O_WRONLY);
}

void sendMessage(pid_t receiver_pid, const char *message) {
	union sigval sv;
	sv.sival_ptr = (void*)message;
	
	if(sigqueue(receiver_pid, SIGUSR1, sv) == -1) {
		perror("Error sending signal");
		exit(EXIT_FAILURE);
	}
}




