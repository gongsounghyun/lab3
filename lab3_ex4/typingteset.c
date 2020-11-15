#include <stdio.h> 
#include <termios.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#define PASSWORDSIZE 12
int main(void) {
	int fd, nread, cnt=0, errcnt=0;
	char ch, ch1, ch2, ch3;
	float alltyping = 0;
	char text[] = "The magic thing is that you can change it.", text1[] = "My name is honggildong.", text2[] = "My University collage is dongeui.", text3[] = "I love Korea!.";
	time_t start = 0, end = 0;
	struct termios init_attr, new_attr;
	fd = open(ttyname(fileno(stdin)), O_RDWR); 
	tcgetattr(fd, &init_attr);
	new_attr = init_attr;
	new_attr.c_lflag &= ~ICANON;
	new_attr.c_lflag &= ~ECHO;
	new_attr.c_cc[VMIN] = 1; new_attr.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
		fprintf(stderr, "터미널 속성을 설정할 수 없음.\n"); 
	}
	time(&start);
	printf("\n\n다음 문장을 그대로 입력하세요.\n%s\n", text); 
	while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') {
		if (ch == text[cnt++])  write(fd, &ch, 1);
		else {
			write(fd, "*", 1); 
			errcnt++;
		}
	}
	alltyping += cnt;
	cnt=0;
	printf("\n\n다음 문장을 그대로 입력하세요.\n%s\n", text1); 
	while ((nread=read(fd, &ch1, 1)) > 0 && ch1 != '\n') {
		if (ch1 == text1[cnt++])  write(fd, &ch1, 1);
		else {
			write(fd, "*", 1); 
			errcnt++;
		}
	}
	alltyping += cnt;
	cnt = 0;
	printf("\n\n다음 문장을 그대로 입력하세요.\n%s\n", text2); 
	while ((nread=read(fd, &ch2, 1)) > 0 && ch2 != '\n') {
		if (ch2 == text2[cnt++])  write(fd, &ch2, 1);
		else {
			write(fd, "*", 1); 
			errcnt++;
		}
	}
	alltyping += cnt;
	cnt = 0;
	printf("\n\n다음 문장을 그대로 입력하세요.\n%s\n", text3); 
	while ((nread=read(fd, &ch3, 1)) > 0 && ch3 != '\n') {
		if (ch3 == text3[cnt++])  write(fd, &ch3, 1);
		else {
			write(fd, "*", 1); 
			errcnt++;
		}
	}
	alltyping += cnt;
	time(&end);
	printf("\n타이핑 오류의 횟수는 %d\n", errcnt); 
	printf("분당 타이핑속도는 %.3f\n", 60 * (alltyping/(end-start)));
	tcsetattr(fd, TCSANOW, &init_attr);
	close(fd);
}
 
