// Omer Shoulstein 214668840

#include <stdio.h>
#include <fcntl.h>
#include "unistd.h"
#include "string.h"
#include "dirent.h"
#include "stdlib.h"
#include "sys/wait.h"


#define NO_C_FILE "0"
#define COMPILATION_ERROR "10"
#define WRONG "50"
#define SIMILAR "75"
#define EXCELLENT "100"


void grade(char *name, char *note, char *grade, int resultFile) {
    write(resultFile, name, strlen(name));
    write(resultFile, ",", 1);
    write(resultFile, grade, strlen(grade));
    write(resultFile, ",", 1);
    write(resultFile, note, strlen(note));
    write(resultFile, "\n", 1);
}


void checkStudent(DIR *dir, char *studentName, char *name, int inFile, char *expectedOut, int errFile,
                  char *testFileName, int resultFile) {
    struct dirent *entry;
    size_t nameLength;
    int foundFile = 0;
    while ((entry = readdir(dir)) != NULL) {
        nameLength = strlen(entry->d_name);
        if (nameLength < 2) {
            continue;
        }
        if (strcmp(entry->d_name + nameLength - 2, ".c") == 0) {
            foundFile = 1;
            char *args[] = {"gcc", strcat(strcat(name, "/"), entry->d_name), "-o", entry->d_name, NULL};
            int pid = fork(), success;
            if (pid == 0) {
                dup2(errFile, STDERR_FILENO);
                success = execvp(args[0], args);
                exit(success);
            } else {
                wait(&success);
                if (WIFEXITED(success)) {
                    success = WEXITSTATUS(success);
                }
            }
            if (success != 0) {
                grade(studentName, "COMPILATION_ERROR", COMPILATION_ERROR, resultFile);
                return;
            }
            char fullName[152] = "./";
            strcat(fullName, entry->d_name);
            char *fileArgs[] = {fullName, NULL};
            pid = fork();
            if (pid == 0) {
                lseek(inFile, 0, SEEK_SET);
                dup2(inFile, STDIN_FILENO);
                int testFile = open(testFileName, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                dup2(testFile, STDOUT_FILENO);
                dup2(errFile, STDERR_FILENO);
                success = execvp(fileArgs[0], fileArgs);
                close(testFile);
                exit(0);
            } else {
                wait(NULL);
            }
            pid = fork();
            char *compArgs[] = {"./comp.out", testFileName, expectedOut, NULL};
            if (pid == 0) {
                success = execvp(compArgs[0], compArgs);
            } else {
                wait(&success);
                if (WIFEXITED(success)) {
                    success = WEXITSTATUS(success);
                }
            }
            switch (success) {
                case 1:
                    grade(studentName, "EXCELLENT", EXCELLENT, resultFile);
                    break;
                case 2:
                    grade(studentName, "WRONG", WRONG, resultFile);
                    break;
                case 3:
                    grade(studentName, "SIMILAR", SIMILAR, resultFile);
                    break;
            }
            remove(entry->d_name);
            break;
        }
    }
    if (!foundFile) {
        grade(studentName, "NO_C_FILE", NO_C_FILE, resultFile);
    }
}

int main(int argc, char *argv[]) {

    int conf = open(argv[1], O_RDONLY);
    if (conf == -1) {
        write(1, "Error in: open\n", 15);
        return 1;
    }

    int errFile = open("errors.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (errFile == -1) {
        write(1, "Error in: open\n", 15);
        return 1;
    }

    int resultFile = open("results.csv", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (resultFile == -1) {
        write(1, "Error in: open\n", 15);
        return 1;
    }

    char temp[2] = {'\0', '\0'};
    char folder[150] = "";
    char tempFolder[150] = "";
    char input[150] = "";
    char output[150] = "";
    while (read(conf, temp, 1) > 0) {
        if (temp[0] == '\n') {
            break;
        }
        strcat(folder, temp);
    }
    while (read(conf, temp, 1) > 0) {
        if (temp[0] == '\n') {
            break;
        }
        strcat(input, temp);
    }
    while (read(conf, temp, 1) > 0) {
        if (temp[0] == '\n') {
            break;
        }
        strcat(output, temp);
    }
    close(conf);

    int inFile = open(input, O_RDONLY);
    if (inFile == -1) {
        write(1, "Input file not exist\n", 21);
        return -1;
    }
    int outFile = open(output, O_RDONLY);
    if (outFile == -1) {
        write(1, "Output file not exist\n", 22);
        return -1;
    }
    close(outFile);

    DIR *dir = opendir(folder);
    if (dir == NULL) {
        write(1, "Not a valid directory\n", 22);
        return -1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        strcpy(tempFolder, folder);
        DIR *studentDir = opendir(strcat(strcat(tempFolder, "/"), entry->d_name));
        if (studentDir == NULL) {
            continue;
        }
        strcpy(tempFolder, folder);
        checkStudent(studentDir, entry->d_name, strcat(strcat(tempFolder, "/"), entry->d_name), inFile, output, errFile,
                     "test.txt", resultFile);
    }
    closedir(dir);
    close(errFile);
    close(inFile);
    close(resultFile);
    return 0;
}