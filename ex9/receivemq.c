/* receivemq.c */
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

struct msgq_data recv_data;

int main()
{
    int qid, len;
        if((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
            perror ("msgget failed");
            exit(1);
        }
    while(1){
        printf("received from message queue: ");
        if ((len=msgrcv(qid, &recv_data, BUFSIZE, 0, 0 )) == -1) {
            perror ("msgrcv failed");
            exit(1);
        }

        for (int i = 0; i < strlen(recv_data.text); ++i) {
            if (recv_data.text[i] == '\n') {
                recv_data.text[i] = '\0';
            }
        }

        recv_data.text[len] = '\0';
        printf("%s", recv_data.text);
        printf("\n");

        if (strcmp(recv_data.text, "fin") == 0) {
            printf("finish the program.\n");
            break;
        }
    }
    return 0;
}