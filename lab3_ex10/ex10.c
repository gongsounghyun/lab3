#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define MAX_READ 10
#define SEMKEY (key_t)0111
int src_fd;
int dst_fd;
char buf[MAX_READ];
ssize_t rcnt;
ssize_t tot_cnt = 0;

void testsem(int semid);
void p(int semid);
void v(int semid);

int main(int argc, char *argv[]){
	int semid, i;

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	
	if (argc < 3) {
		fprintf(stderr, "Usage: file_copy src_file dest_file\n");
		exit(1);
	}
	if ((src_fd = open(argv[1], O_RDONLY)) == -1 ){
		perror("src open");
		exit(1);
	}
	if ( (dst_fd = creat(argv[2], mode)) == -1 ){
		perror("dst open");
		exit(1);
	}

	union semun {
		int value;
		struct semid_ds *buf;
		unsigned short int *array;
	} arg;

	if ((semid = semget(SEMKEY, 1, IPC_CREAT | 0666)) == -1) {
		perror ("semget failed");
		exit (1);
	}
	arg.value = 1;

	if (semctl(semid, 0, SETVAL, arg) == -1) {
		perror ("semctl failed");
		exit (1);
	}

	if (!fork())
		testsem(semid);

	sleep(10);

	if (semctl(semid, 0, IPC_RMID, arg) == -1) {
		perror ("semctl failed");
		exit (1);
	}

	if (rcnt < 0) {
		perror("read");
		exit(1);
	}
	printf("total write count = %ld\n", tot_cnt);
	close(src_fd);
	close(dst_fd);
}

void testsem (int semid)
{

	srand((unsigned int) getpid());
	p(semid);
	printf("process %d : semaphore in use\n", getpid());
	
	while ( (rcnt = read(src_fd, buf, MAX_READ)) > 0) {
		tot_cnt += write(dst_fd, buf, rcnt);
	}

	sleep(rand()%5);
	printf("process %d : putting semaphore\n", getpid());
	v(semid);
	exit(0);
}
void p (int semid)
{
	struct sembuf pbuf;
	pbuf.sem_num = 0;
	pbuf.sem_op = -1;
	pbuf.sem_flg = SEM_UNDO;
	if (semop (semid, &pbuf, 1) == -1) {
		perror ("semop failed");
		exit (1);
	}
}
void v (int semid)
{
	struct sembuf vbuf;
	vbuf.sem_num = 0;
	vbuf.sem_op = 1;
	vbuf.sem_flg = SEM_UNDO;
	if (semop (semid, &vbuf, 1) == -1) {
		perror ("semop failed");
		exit (1);
	}
}

