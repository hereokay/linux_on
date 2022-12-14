#include "smallsh.h"

static char inpbuf[MAXBUF];
static char tokbuf[2*MAXBUF];

static char *ptr = inpbuf;
static char *tok = tokbuf;

														 
static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

int userin(char* p){
	int c, count;
	ptr = inpbuf;
	tok = tokbuf;

	char cw_dir[MAXBUF];
        getcwd(cw_dir,MAXBUF);
        struct passwd *pwd;
        pwd = getpwuid(geteuid());
        char* home_dir = pwd->pw_dir;

        int flag= 0;
        if(strlen(home_dir) < strlen(cw_dir)){
                // empty
        }else{
                for(int i=0;i<strlen(home_dir);i++){
                        if(home_dir[i] != cw_dir[i]){
                                flag=1;
                        }
                }
        }


	if(flag == 0){
                cw_dir[0]='~';
                for(int i=0;i<strlen(cw_dir)-strlen(home_dir);i++){
                        cw_dir[i+1]=cw_dir[i+strlen(home_dir)];
                }
                cw_dir[strlen(cw_dir)-strlen(home_dir)+1]='\0';
        }
	
	printf("%s$ ", cw_dir);
	count = 0;

	while(1){
		if((c = getchar()) == EOF)
			return EOF;
		if(count < MAXBUF)
			inpbuf[count++] = c;
		if(c == '\n' && count < MAXBUF){
			inpbuf[count] = '\0';
			return count;
		}
		if(c == '\n' || count >= MAXBUF){
			printf("smallsh: input line is too long\n");
			count = 0;
			printf("%s", p);
		}
	}
}


int gettok(char** outptr){
	int type;
	*outptr = tok;
	while(*ptr == ' ' || *ptr == '\t')
		ptr++;

	*tok++ = *ptr;
	switch(*ptr++){
		case '\n':
			type = EOL;
			break;
		case '&':
			type = AMPERSAND;
			break;
		case ';':
			type = SEMICOLON;
			break;
		default:
			type = ARG;
			while(inarg(*ptr))
				*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return type;
}

int inarg(char c){
	char *wrk;

	for(wrk = special; *wrk; wrk++){
		if(c == *wrk)
			return 0;
	}
	return 1;
}

void procline(){
	char *arg[MAXARG + 1];
	int toktype, type;
	int narg = 0;
	for(;;){
		switch(toktype = gettok(&arg[narg])){
			case ARG:
				if (narg < MAXARG)
					narg++;
				break;
			case EOL:
			case SEMICOLON:
			case AMPERSAND:
				if (toktype == AMPERSAND) type = BACKGROUND;
				else type = FOREGROUND;
				if (narg != 0){
						arg[narg] = NULL;
						runcommand(arg, type);
				}
				if (toktype == EOL) return;
				narg = 0;
				break;
		}
	}
}

int runcommand(char** cline, int where){
	pid_t pid;
	int status;
	int fd_new;
	int redi = 0;

	if(strcmp(*cline, "exit") == 0){
		exit(0);
	}

	if(strcmp(*cline, "cd") == 0){
		
		if(cline[1] ==NULL){
			struct passwd *pwd;
    			pwd = getpwuid(geteuid());
     			char* home_dir = pwd->pw_dir;
			cline[1]=home_dir;	
			if(chdir(cline[1]) == -1)
				perror(cline[1]);
			return 0;
		}
		if(cline[2] != NULL){
			printf("Usage: cd <dir>\n");
			return 0;
		}
		if(cline[1][0]=='~'){

			char cw_dir[100];
        		getcwd(cw_dir,100);
        		struct passwd *pwd;
        		pwd = getpwuid(geteuid());
        		char* home_dir = pwd->pw_dir;

        		int length_line = strlen(cline[1]);
        		for(int i=0;i<length_line-1;i++){
        	       		cline[1][i]= cline[1][i+1];
       			}
	        	cline[1][length_line-1]='\0';
	        	strcat(home_dir,cline[1]);
			cline[1] = home_dir;
		}

		if(chdir(cline[1]) == -1)
			perror(cline[1]);
		return 0;
	}
	
	for(int i = 0; cline[i]; i++){

		if(strcmp(cline[i],">")==0){
			if((fd_new = open(cline[i + 1], O_WRONLY | O_CREAT, 0755)) == -1){
				perror(cline[i + 1]);
				return -1;
			}
			redi = 1;
			cline[i] = NULL;
			break;
		}
	}

    	switch(pid = fork()){
        	case -1:
            		perror("smallsh");
            		return -1;
        	case 0:
	    		if(redi == 1){
				if(dup2(fd_new, 1) == -1)
					perror("redi");
				}
			execvp(*cline, cline);
            		perror(*cline);
            		exit(1);
	}
	if (where == BACKGROUND){
		printf("[Process id] %d\n", pid);
		return 0;
	}
	if(waitpid(pid, &status, 0)==-1)
		return -1;
	else
		return status;
}
