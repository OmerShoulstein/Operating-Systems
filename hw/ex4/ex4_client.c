#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void timeout(int sig) {
    printf("The client was closed because no service request was received for the last 30 seconds.\n");
    exit(0);
}

void getAnswer(int sig) {
    FILE *f;
    f = fopen("to_srv", "r");
    char *line = NULL;
    size_t len;
    getline(&line, &len, f);
    fclose(f);
    printf("%s", line);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Enter 4 arguments\n");
        exit(1);
    }
    int serverPid = atoi(argv[1]);
    int firstArg = atoi(argv[2]);
    int operation = atoi(argv[3]);
    int secondArg = atoi(argv[4]);
    FILE *f;
    if (!access("to_srv", F_OK)) {
        printf("The File %s was Found\n", "to_srv");
        return 0;
    } else {
        alarm(30);
        printf("The File %s was not Found\n", "to_srv");
        f = fopen("to_srv", "w");
        fprintf(f, "%d\n", getpid());
        fprintf(f, "%d\n", firstArg);
        fprintf(f, "%d\n", operation);
        fprintf(f, "%d\n", secondArg);
        fclose(f);
        getAnswer(SIGUSR1);
        signal(SIGALRM, timeout);
        signal(SIGUSR1,getAnswer);
        kill(serverPid, SIGUSR1);
        alarm(30);
        pause();
    }


    return 0;
}
