//Omer Shoulstein 214668840


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>


typedef struct command {
    char text[100];
    int pid;
} command;


void history(command *commands, int commandCount) {
    int i = 0;
    for (i = 0; i < commandCount; i++) {
        printf("%d %s\n", commands[i].pid, commands[i].text);
    }
}

void addToHistory(int pid, char *text, command *commands, int *commandCount) {
    command newCommand;
    newCommand.pid = pid;
    strcpy(newCommand.text, text);
    commands[*commandCount] = newCommand;
    (*commandCount)++;
}

int parseArguments(char *command, char *arguments[100]) {
    int i;
    for (i = 0; i < 100; i++) {
        arguments[i] = NULL;
    }
    char *token = strtok(command, " ");
    i = 0;
    while (token != NULL) {
        arguments[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    return i;
}

void cd(char **commandParams) {
    if (chdir(commandParams[1]) < 0) {
        perror("cd failed");
    }
}

int main(int argc, char *argv[]) {
    char *path = getenv("PATH");
    int i;
    for (i = 1; i < argc; i++) {
        strcat(path, ":");
        strcat(path, argv[i]);
    }
    setenv("PATH", path, 1);
    command commands[100];
    int commandCount = 0;
    char currCommand[100];
    char commandText[100];
    char *commandParams[100];
    while (1) {
        printf("$ ");
        fflush(stdout);
        scanf(" %[^\n]%*c", currCommand);
        if (strcmp(currCommand, "exit") == 0) {
            break;
        }
        if (strcmp(currCommand, "history") == 0) {
            addToHistory(getpid(), currCommand, commands, &commandCount);
            history(commands, commandCount);
            continue;
        }
        char fullCommand[100];
        strcpy(fullCommand, currCommand);
        parseArguments(currCommand, commandParams);
        strcpy(commandText, commandParams[0]);
        if (strcmp(commandText, "cd") == 0) {
            addToHistory(getpid(), fullCommand, commands, &commandCount);
            cd(commandParams);
        } else {
            //fork
            int pid = fork();
            if (pid == 0) {
                //child
                int success = execvp(commandText, commandParams);
                if (success == -1) {
                    perror(strcat(commandText, " failed"));
                    exit(1);
                }
                //kill the process
                exit(0);
            } else {
                //parent
                addToHistory(pid, fullCommand, commands, &commandCount);
                wait(NULL);
            }
        }
    }
    return 0;
}