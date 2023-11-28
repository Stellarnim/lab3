#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int my_system(const char *command) {
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);

        // execl이 실패하면 아래 코드가 실행됨
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid < 0) { // fork 실패
        perror("fork");
        exit(EXIT_FAILURE);
    } else { // 부모 프로세스
        // 자식 프로세스가 종료되기를 기다림
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        // 자식 프로세스의 종료 상태 반환
        return WEXITSTATUS(status);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char command[4096];
    strcpy(command, argv[1]);
    if (argc >= 3){
        for (int i = 2; i < argc; ++i) {
        strcat(command, " ");
        strcat(command, argv[i]);
        }
    }
    // 명령어를 my_system 함수를 통해 실행
    int result = my_system(command);

    // 실행 결과 출력
    printf("Command exited with status %d\n", result);

    return EXIT_SUCCESS;
}