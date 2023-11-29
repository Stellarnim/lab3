/* sendmq.c */
/* message queue example */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 64
#define QKEY (key_t)0111

struct msgq_data {
    long type;
    char text[BUFSIZE];
};
int main()
{
    int qid, len;
    char buf[BUFSIZE];
    if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
        perror ("msgget failed");
        exit(1);
    }

    while(1){
        printf("send message: ");
        fgets(buf, sizeof(buf), stdin);
        struct msgq_data send_data = {1, ""};
        for (int i = 0; i < strlen(buf); ++i) {
            if (buf[i] == '\n') {
                buf[i] = '\0';
            }
        }
        strcpy(send_data.text, buf);
        if (strcmp(send_data.text, "fin") == 0) {
            printf("finish the program.\n");
            msgsnd(qid, &send_data, sizeof(send_data.text), 0);
            break;
        }

        if(msgsnd(qid, &send_data, sizeof(send_data.text), 0) == -1) {
            perror ("msgsend failed");
            exit(1);
        }
    }

    return 0;
}
