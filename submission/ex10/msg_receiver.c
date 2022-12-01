#include "msg_header.h"
#include <signal.h>

int msgid;
struct msg_entry msg;
int msglen;

void int_handler(int signo){
//	while((msglen = msgrcv(msgid,&msg,MAXLEN,-10,IPC_NOWAIT))>0){
		
//	}
	msgctl(msgid,IPC_RMID,&msg);
	
//	perror("msgctl");

	exit(0);
}

int main(){
	key_t key;

	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_handler = int_handler;
	sigaction(SIGINT,&act,NULL);

	key = ftok("msg_header.h",1);
	msgid = msgget(key, IPC_CREAT | QPERM);
	if(msgid == -1){
		perror("msgget");
		return 1;
	}
	while((msglen = msgrcv(msgid,&msg,MAXLEN,-10,IPC_NOWAIT))>0){
		printf("Received Message = %s\n",msg.mtext);
		sleep(1);
	}

	msgctl(msgid,IPC_RMID,&msg);
	return 0;
}
