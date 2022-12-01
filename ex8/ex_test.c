#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void child_handler(int a){
	int b=0;
	waitpid(0,NULL,NULL);
}

int main(){
	struct sigaction act;
	act.sa_handler = child_handler;
	sigfillset(&act.sa_mask);
	sigaction(SIGCHLD,&act,NULL);
	fork();
	fork();
	for(;;);
	return 0;
}
