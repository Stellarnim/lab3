#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>

#define SHMSIZE 1024
#define SHMNAME "/copyfile"
#define SEMNAME "/copy_sem"
#define SHMKEY (key_t)0111

void p(sem_t *semd);
void v(sem_t *semd);
void testpsem(sem_t *semd, char *shmaddr, int i);

int main(){
    sem_t *semd;
    int i, val;
    if ((semd = sem_open(SEMNAME, O_CREAT, 0600, 1)) == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }
    sem_getvalue(semd, &val);

    printf("semaphore is %d\n", val);

    int shmid;
    char *shmaddr;
    if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shmaddr = shmat(shmid, 0, 0);

    for (i = 2; i < 4; i++) {
        if (!fork())
        testpsem(semd, shmaddr, i);
    }

    sleep(10);

    if (sem_close(semd) == -1) {
        perror("sem_close failed");
        exit(1);
    }

    if (sem_unlink(SEMNAME) == -1) {
        perror("sem_unlink failed");
        exit(1);
    }
    if (shmdt(shmaddr) == -1) {
        perror ("shmdt failed");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror ("shmctl failed");
        exit(1);
    }
}

void testpsem (sem_t *semd, char *shmaddr, int i)
{
    srand((unsigned int) getpid());
    p(semd);
    if (getpid() != 0) {
        FILE *file = fopen("source1.txt", "r");
        fread(shmaddr, 1, SHMSIZE, file);
        fclose(file);
    }
    printf("process %d : semaphore in use\n", getpid());
    printf("%s", shmaddr);
    sleep(rand()%5);
    if (getpid() != 0) {
        char filename[20];
        sprintf(filename, "source%d.txt", i);
        FILE *file = fopen(filename, "w");
        fprintf(file, "%s", shmaddr);
        fclose(file);
    }
    printf("process %d : putting semaphore\n", getpid());
    v(semd);
    exit(0);
}

void p (sem_t *semd)
{
    int ret;
    if ((ret = sem_trywait(semd)) != 0 && errno == EAGAIN)
        sem_wait(semd);
    else if (ret != 0) {
        perror("sem_trywait failed");
        exit(1);
    }
}

void v(sem_t *semd)
{
    if (sem_post(semd) != 0) {
        perror("sem_post failed");
        exit(1);
    }
}
