/* simplesh.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_ARGS 50
#define MAX_INPUT 256

int getargs(char *cmd, char **argv);

void command_exit();
void handle_signal(int signo){
    if(signo == SIGINT){
        printf("\nCtrl-C (SIGINT)\n");
        kill(getpid(), SIGTERM);
    }
    else if(signo == SIGTSTP){
        printf("\nCtrl-Z (SIGQUIT)\n");
        kill(getpid(), SIGSTOP);
    }
}
int main() {
    char buf[MAX_INPUT];
    char *argv[MAX_ARGS];
    pid_t pid;
    int background;

    signal(SIGINT, handle_signal);
	signal(SIGTSTP, handle_signal);

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

        background = 0;
        if (narg > 0 && strcmp(argv[narg - 1], "&") == 0) {
            background = 1;
            argv[--narg] = NULL; // 인수에서 '&' 제거
        }

        pid = fork();

        if (pid == 0) {
            if(background){
                setsid();
                printf("run background.");
            }
            execvp(argv[0], argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            if (!background){
                waitpid(pid, NULL, 0);
            }
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
    printf("shell program close.\n");
    exit(0);
}