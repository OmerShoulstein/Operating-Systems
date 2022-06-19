#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void timeout(int sig) {
    printf("The server was closed because no service request was received for the last 60 seconds.\n");
    exit(0);
}

void getParams(int *pid, int *first, int *operation, int *second) {
    FILE *f;
    f = fopen("to_srv", "r");
    char *line = NULL;
    size_t len;
    getline(&line, &len, f);
    *pid = atoi(line);
    getline(&line, &len, f);
    *first = atoi(line);
    getline(&line, &len, f);
    *operation = atoi(line);
    getline(&line, &len, f);
    *second = atoi(line);
    fclose(f);
    remove("to_srv");
}

void clientHandler(int sig) {
    signal(SIGUSR1,clientHandler);
    int pid = fork();
    if (pid){
        return;
    }
    signal(SIGUSR1, clientHandler);
    int clientPid, first, operation, second;
    getParams(&clientPid, &first, &operation, &second);
    if (0 == clientPid) {
        return;
    }
    int result;
    switch (operation) {
        case 1:
            result = first + second;
            break;
        case 2:
            result = first - second;
            break;
        case 3:
            result = first * second;
            break;
        case 4:
            result = first / second;
            break;
    }
    FILE *f;
    char clientFile[16];
    strcpy(clientFile, "to_client_");
    sprintf(clientFile + strlen(clientFile), "%d", clientPid);
    f = fopen(clientFile, "w");
    fprintf(f, "%d\n", result);
    fclose(f);
    kill(clientPid, SIGUSR1);
    exit(0);
}


int main() {
    printf("%d\n", getpid());
    signal(SIGALRM, timeout);
    signal(SIGUSR1, clientHandler);
    while (1) {
        alarm(60);
        pause();
    }
}
