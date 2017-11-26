#include <signal.h>
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h>  
#include <time.h>

#define SLEEPPER 3
int semaphore_p(int sem_id, int sem_num);
int semaphore_v(int sem_id, int sem_num);

int initsem(int val1,int val2);
int sem_id;

void sleepRand(float per);

void closeProgress(int sig){
	semctl(sem_id, 0, IPC_RMID, NULL);
	kill(0,9);
	exit(0);
}


int main(){
	srand(time(0));
	sem_id = initsem(0,0);
	signal(SIGINT,closeProgress);
	int pid = fork();
	if(pid==0){ // ticket
		while(1){
			printf("close the door.\n");
			semaphore_v(sem_id,0);
			printf("sale tickets.\n");
			sleepRand(SLEEPPER);
			semaphore_p(sem_id,1);
			printf("Open the door.\n");
			printf(".........in & out..........\n");
		}
	}else{// car
		while(1){
			semaphore_p(sem_id,0);
			printf("Start car....\n");
			sleepRand(SLEEPPER);
			printf("running....\n");
			sleepRand(SLEEPPER);
			printf("Stop the car...\n");
			semaphore_v(sem_id,1);
		}
	}
	semctl(sem_id, 0, IPC_RMID, NULL);
	return 0;
}
int initsem(int val1,int val2){
	int sem_id = semget(IPC_PRIVATE, 2, 0666 | IPC_CREAT);
	if(sem_id<0){
		printf("sem_id error.\n");
		exit(1);		
	}
	if(semctl(sem_id, 0, SETVAL, val1) < 0){
		printf("initsem 1 error.\n");
		exit(1);
	}
	if(semctl(sem_id, 1, SETVAL, val2) < 0){
		printf("initsem 2 error.\n");
		exit(1);
	}
	return sem_id;
}

int semaphore_p(int sem_id, int sem_num){  
    //对信号量做减1操作，即等待P（sv）  
    struct sembuf sem_b;  
    bzero(&sem_b, sizeof(sem_b));
    sem_b.sem_num = sem_num;  
    sem_b.sem_op = -1;//P()  
    sem_b.sem_flg = SEM_UNDO;  
    int err;  
    if((err = semop(sem_id, &sem_b, 1)) == -1)
    {  
        fprintf(stderr, "semaphore_p failed %d\n",err);  
        return 0;  
    }  
    return 1;  
}  
  
int semaphore_v(int sem_id, int sem_num){  
    //这是一个释放操作，它使信号量变为可用，即发送信号V（sv）  
    struct sembuf sem_b;  
    bzero(&sem_b, sizeof(sem_b));
    sem_b.sem_num = sem_num;  
    sem_b.sem_op = 1;//V()  
    sem_b.sem_flg = SEM_UNDO;
    int err;  
    if((err = semop(sem_id, &sem_b, 1)) == -1)  
    {  
        fprintf(stderr, "semaphore_v failed %d\n",err);  
        return 0;  
    }  
    return 1;  
} 
void sleepRand(float per){
	int times = (int)(rand()%1000000)*per;
	usleep(times);
	// printf("sleep %d",times);
}