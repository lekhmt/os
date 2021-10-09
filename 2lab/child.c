#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CHILD  "[%d] CHILD. "

void handle_error(bool expr, char* msg) {
    if (expr) {
        perror(msg);
        exit(-1);
    }
}

int main(int argv, char* argc[]){

    char* name = argc[0];
    FILE* output = fopen(name, "w+");
    handle_error(output == NULL, "file error");

    int src_fd[2], err_fd[2];
    src_fd[0] = atoi(argc[1]);
    src_fd[1] = atoi(argc[2]);
    err_fd[0] = atoi(argc[3]);
    err_fd[1] = atoi(argc[4]);

    char str[256];
    read(src_fd[0], &str, sizeof(str));
    read(src_fd[0], &str, sizeof(str));
    while(strcmp(str, "quit") != 0){
        bool err;
        if (str[0] >= 'A' && str[0] <= 'Z'){
            err = false;
            fprintf(output, "%s\n", str);
        } else {
            err = true;
        }
        write(err_fd[1], &err, sizeof(bool));
        read(src_fd[0], &str, sizeof(str));
    }

}
