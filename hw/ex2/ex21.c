// Omer Shoulstein 214668840

#include "stdio.h"
#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"
#include "ctype.h"

int main(int argc, char **argv) {
    int file1 = open(argv[1], O_RDONLY);
    if (file1 < 0) {
        write(1, "Error in: open\n", 19);
        exit(-1);
    }
    int file2 = open(argv[2], O_RDONLY);
    if (file2 < 0) {
        write(1, "Error in: open\n", 19);
        exit(-1);
    }

    char c1;
    char c2;
    short status = 1;
    while (read(file1, &c1, 1)) {
        if (0 == read(file2, &c2, 1)) {
            do {
                if (c1 == ' ' || c1 == '\n') {
                    status = 3;
                } else {
                    status = 2;
                    break;
                }
            } while (read(file1, &c1, 1));
            break;
        }
        if (c1 == c2) {
            continue;
        }
        while ((c1 == ' ' || c1 == '\n') && read(file1, &c1, 1)) {
        }
        while ((c2 == ' ' || c2 == '\n') && read(file2, &c2, 1)) {
        }
        if (tolower(c1) == tolower(c2)) {
            status = 3;
        } else {
            status = 2;
            break;
        }

    }
    while (read(file2, &c2, 1)) {
        if (c2 == ' ' || c2 == '\n') {
            status = 3;
        } else {
            status = 2;
            break;
        }
    }
    close(file1);
    close(file2);
    return status;
}