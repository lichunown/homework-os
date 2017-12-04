#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXLEN 1024


void thread_read();
void thread_print();
pthread_mutex_t mut;
char* p_data;
void thread_read(){
	while(1){
		char* data = (char*)malloc(MAXLEN);
		fgets(data,MAXLEN,stdin);
		pthread_mutex_lock(&mut);
		p_data = data;
		pthread_mutex_unlock(&mut);
	}
}
void thread_print(){
	while(1){
		pthread_mutex_lock(&mut);
		if(p_data!=NULL){
			printf("data in memory:\n%s\n\n",p_data);
			p_data = NULL;
		}
		pthread_mutex_unlock(&mut);
		sleep(1);
	}
}

int main(){
	p_data = NULL;
	 pthread_t p_r,p_p;
	if(pthread_create(&p_r, NULL, (void *)thread_read, NULL)<0){
		printf("[ERROR]: thread `thread_read` create error.\n");
		exit(1);
	}
	if(pthread_create(&p_p, NULL, (void *)thread_print, NULL)<0){
		printf("[ERROR]: thread `thread_print` create error.\n");	
		exit(1);	
	}
	pthread_join(p_r,NULL);
	pthread_join(p_p,NULL);
	return 0;
}