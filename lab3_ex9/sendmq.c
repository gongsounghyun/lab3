#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFSIZE 16
#define QKEY (key_t)0111

struct msgq_data { 
	long type;
	char text[BUFSIZE]; 
};

int main() {
	int qid, len; 
	long count=1;
	char word[16];
	char buf[BUFSIZE];
	
	printf("전송하고자 하는 문자열을 입력\n");
	scanf("%s",word);
	
	struct msgq_data send_data;
	send_data.type = 1;
	strcpy(send_data.text, word);
	
	if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
		perror ("msgget failed");
		exit (1);
	}	
	if (msgsnd(qid,&send_data,strlen(send_data.text),0) == -1) {
		perror ("msgsnd failed");
		exit (1); 
	}
}
