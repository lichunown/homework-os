#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#define MAXSTRSIZE 80
// 1 write; 0 read;

int main(){
	int pipe_p2c[2]; // parent to child
	int pipe_c2p[2]; // child to parent
	if(pipe(pipe_p2c)==-1 || pipe(pipe_c2p)==-1){
		printf("pipe create error.\n");
		exit(1);
	}
	int pid = fork();
	if(pid<0){
		printf("fork error.   [pid]: %d\n",pid);
		exit(1);		
	}
	if(pid==0){// child
		printf("[child] child process created.  pid = %d\n",pid);
		//signal(SIGINT, sigint_func);//TODO
		close(pipe_p2c[1]);
		close(pipe_c2p[0]);
		sleep(1);
		char recvdata[MAXSTRSIZE];
		while(1){
			bzero(recvdata, MAXSTRSIZE);
			if(read(pipe_p2c[0], recvdata, MAXSTRSIZE) > 0){
				printf("[child]: #recv# `%s`\n",recvdata);
				sprintf(recvdata,"child recv data len %ld.", strlen(recvdata));
				write(pipe_c2p[1], recvdata, MAXSTRSIZE);				
			}
		}
	}else{// parent
		printf("[parent] pid = %d\n",pid);
		close(pipe_p2c[0]);
		close(pipe_c2p[1]);
		char input[MAXSTRSIZE];
		sleep(1);
		while(1){
			bzero(input, MAXSTRSIZE);
			printf("[parent input]: >>> ");
			fgets(input, MAXSTRSIZE, stdin);
			if(strcmp(input, "#close\n")==0){
				kill(0,9);
				close(pipe_p2c[1]);
				close(pipe_c2p[0]);
				exit(0);
			}
			write(pipe_p2c[1], input, MAXSTRSIZE);
			bzero(input, MAXSTRSIZE);
			if(read(pipe_c2p[0], input, MAXSTRSIZE) > 0){
				printf("[parent]: #recv# `%s`\n\n\n", input);
			}
		}
	}
	return 0;
}