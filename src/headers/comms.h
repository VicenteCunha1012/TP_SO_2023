#ifndef COMMS_H
#define COMMS_H

void handleSignal(int signo, siginfo_t *info, void *context);
void initCommunication();
void sendMessage(pid_t receiver_pid, const char *message);

#endif
