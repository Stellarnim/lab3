/* simplesh.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 50
#define MAX_INPUT 256

int getargs(char *cmd, char **argv);
void command_exit();

int main() {
    char buf[MAX_INPUT];
    char *argv[MAX_ARGS];
    pid_t pid;

    while (1) {
        printf("shell> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            perror("입력 오류");
            exit(EXIT_FAILURE);
        }

        buf[strcspn(buf, "\n")] = '\0'; // Remove trailing newline

        if (strcmp(buf, "exit") == 0) {
            command_exit();
        }

        int narg = getargs(buf, argv);
        pid = fork();

        if (pid == 0) {
            execvp(argv[0], argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("fork failed");
        }
    }
}

int getargs(char *cmd, char **argv) {
    int narg = 0;
    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
                cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}

void command_exit(){
    printf("shell 종료.\n");
    exit(EXIT_SUCCESS);
}