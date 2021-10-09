// Леухин М.В. М8О-206Б-20
// Вариант 15

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

#define PARENT "[%d] PARENT. "
#define CHILD  "[%d] CHILD. "

void handle_error(bool expr, char* msg) {
    if (expr) {
        perror(msg);
        exit(-1);
    }
}

int main(){

    int src_fd[2];
    int pipe_response = pipe(src_fd);
    handle_error(pipe_response == -1, "pipe error");

    int err_fd[2];
    pipe_response = pipe(err_fd);
    handle_error(pipe_response == -1, "pipe error");

    pid_t id = fork();
    handle_error(id == -1, "fork error");

    if (id == 0){

        char name[64];
        read(src_fd[0], &name, sizeof(name));

        char *src_fd_0, *src_fd_1, *err_fd_0, *err_fd_1;
        asprintf(&src_fd_0, "%d", src_fd[0]);
        asprintf(&src_fd_1, "%d", src_fd[1]);
        asprintf(&err_fd_0, "%d", err_fd[0]);
        asprintf(&err_fd_1, "%d", err_fd[1]);

        execl("child.out", name, src_fd_0, src_fd_1, err_fd_0, err_fd_1, NULL);

    } else {

        printf(PARENT "Enter the name of file to write: ", getpid());
        char name[256];
        fgets(name, 256, stdin); name[strlen(name) - 1] = '\0';
        write(src_fd[1], &name, sizeof(name));

        char str[256];
        printf(PARENT "Enter string: ", getpid());
        fgets(str, 256, stdin); str[strlen(str) - 1] = '\0';
        while (strcmp(str, "quit") != 0){
            write(src_fd[1], &str, sizeof(str));
            bool err;
            read(err_fd[0], &err, sizeof(bool));
            if (err){
                printf(PARENT "Error: \"%s\" is not valid.\n", getpid(), str);
            }
            printf(PARENT "Enter string: ", getpid());
            fgets(str, 256, stdin); str[strlen(str) - 1] = '\0';
        }
        write(src_fd[1], &str, sizeof(str));

    }

    return 0;

}