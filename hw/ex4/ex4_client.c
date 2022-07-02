//Omer Shoulstein 214668840
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void timeout(int sig) {
    printf("The client was closed because no service request was received for the last 30 seconds.\n");
    remove("to_srv");
    exit(0);
}

void getAnswer(int sig) {
    char clientFile[16];
    strcpy(clientFile, "to_client_");
    sprintf(clientFile + strlen(clientFile), "%d", getpid());
    FILE *f;
    f = fopen(clientFile, "r");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, f);
    fclose(f);
    printf("%s", line);
    remove(clientFile);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    int serverPid = atoi(argv[1]);
    int firstArg = atoi(argv[2]);
    int operation = atoi(argv[3]);
    if (operation < 1 || operation > 4) {
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    int secondArg = atoi(argv[4]);
    FILE *f;
    int tries = 0;
    int maxTries = 10;
    while (!access("to_srv", F_OK) && tries < maxTries) {
        int time = (rand() % 5) + 1;
        sleep(time);
        ++tries;
    }
    if (tries == maxTries) {
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    alarm(30);
    f = fopen("to_srv", "w");
    fprintf(f, "%d\n%d\n%d\n%d\n", getpid(), firstArg, operation, secondArg);
    fclose(f);
    signal(SIGALRM, timeout);
    signal(SIGUSR1, getAnswer);
    kill(serverPid, SIGUSR1);
    alarm(30);
    pause();


    return 0;
}
