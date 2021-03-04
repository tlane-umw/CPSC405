#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]){
	if(argc == 1){
		printf("useage: doit <command> <string if pipe is arg>\n");
		exit(-1);
	}
	if(strcmp(argv[1],"ls") == 0){
		pid_t first = fork();
		if(first < 0){
			fprintf(stderr, "fork failed\n");
			exit(-1);	
		}else if(first > 0){
			int wc = wait(NULL);
			assert(wc >= 0);
			printf("parent\n");

		}else{
			execl("/bin/ls","ls",NULL);
		}
	}else if(strcmp(argv[1],"pipe") == 0){
		int pipefd[2];
		char buf[80];
		if(argc < 3){
			printf("incorrect format, useage: doit <pipe> <string>\n");
			exit(-1);
		}
		if(pipe(pipefd) == -1){
			perror("pipe");
			exit(-1);
		}
		pid_t rc[2];
		rc[0] = fork();
		if(rc[0] < 0){
			fprintf(stderr, "fork failed\n");
			exit(-1);
		}else if(rc[0] == 0){
			close(pipefd[0]);
			write(pipefd[1], argv[2], strlen(argv[1]) +1);
			close(pipefd[1]);
			printf("childpid %d finished\n", (int)(getpid()));
		}else if(rc[0] > 0){
			rc[1] = fork();
			if(rc[1] < 0){
				fprintf(stderr, "fork 2 failed\n");
				exit(-1);
			}else if(rc[1] > 0){
				int w2 = wait(NULL);
				int w3 = wait(NULL);
				assert(w2 > 0);
				assert(w3 > 0);
				printf("parent\n");
			}else{
				int n = read(pipefd[0], buf, sizeof(buf));
				buf[n] = '\n';
				buf[n+1] = '\0';
				write(STDOUT_FILENO, buf, n+1);
				close(pipefd[0]);
				printf("childpid %d finished\n", (int)getpid());
			}
		}
	}else{
	    	printf("Wrong command\n");
	    	exit(-1);
	}

return 0;

}
