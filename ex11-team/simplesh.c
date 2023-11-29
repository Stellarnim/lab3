/* simplesh.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_ARGS 50
#define MAX_INPUT_LENGTH 256

int getargs(char *cmd, char **argv);

void handle_signal(int signo);
void command_exit();
void command_ls();
void command_pwd();
void command_cd(char *dirname);
void command_mkdir(char *dirname);
void command_rmdir(char *dirname);
int main() {
    char buf[MAX_INPUT_LENGTH];
    char *argv[MAX_ARGS];
    pid_t pid;
    int background = 0;

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
        else if (strcmp(buf, "ls") == 0) {
            command_ls();
            continue;
        }
        else if (strcmp(buf, "pwd") == 0){
            command_pwd();
            continue;
        }
/*        else if (strcmp(buf, "cd") == 0){
            char *dirname = buf + 3;
            command_cd(dirname);
            continue;
        } */
        else if (strcmp(buf, "mkdir") == 0){
            char *dirname = buf + 6;
            command_mkdir(dirname);
            continue;
        }
        else if (strcmp(buf, "rmdir") == 0){
            char *dirname = buf + 6;
            command_rmdir(dirname);
            continue;
        }


        int narg = getargs(buf, argv);

        if (narg > 0 && strcmp(argv[narg - 1], "&") == 0) {
            background = 1;
            argv[--narg] = NULL; // 인수에서 '&' 제거
        }
        else {
            background = 0;
        }
 //       if (narg > 0 && strcspn())

        pid = fork();

        if (pid == 0) {
            if(background){
                freopen("/dev/null", "r", stdin);
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);
                setsid();
            }
            execvp(argv[0], argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } 
        else if (pid > 0) {
            if (!background){
                waitpid(pid, NULL, 0);
            }
        } 
        else {
            perror("fork failed");
        }
    }
    return 0;
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

void command_ls(){
	DIR *dir = opendir(".");

	if (!dir) {
		perror("open dir failed");
		exit(1);
	}

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        printf("%s", entry->d_name);
			
			printf("    ");
        }
		
	}
    printf("\n");
	closedir(dir);
}

void command_pwd(){
    char pwd[64];
    if (getcwd(pwd, sizeof(pwd)) != NULL){
        printf("%s\n", pwd);
    }
    else{
        perror("pwd error");
    }
}

/*void command_cd(char *dirname){
    if (chdir(dirname) == 0) {
        printf("\n %s/", dirname);
    }
    else {
        perror("chdir failed");
    }
}*/

void command_mkdir(char *dirname){
    if(mkdir(dirname, 0777) == 0){
        printf("\n");
    }
    else{
        perror("mkdir failed");
    }
}

void command_rmdir(char *dirname){
    if(mkdir(dirname, 0777) == 0){
        printf("\n");
    }
    else{
        perror("mkdir failed");
    }
}