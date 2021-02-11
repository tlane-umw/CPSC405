#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#define FN_LEN 256
char filename[FN_LEN];
bool using_h = false;
bool using_f = false;
int main(int argc, char *argv[]) {
	int pid_to_kill = 0;
	if(argc == 1){
		printf("Error - command format is $ my_kill -options pid\n");                                                                        exit(-1);
	}

	int opt;
	while ((opt = getopt(argc, argv, ":f:h")) != -1) {
	     switch(opt){	
		case 'h': using_h = true; break;
		case 'f':
			  using_f = true;
			  strcpy(filename,optarg);
			  break;
		case ':':
			  printf("-f needs a value\n");
			  break;
		case '?':
			  printf("Unknown option\n");
			  exit(-1);
	     }
	}
	if ((argc - optind) != 1) {    
		printf("Error - command format is $ my_kill -options pid\n");    
		exit(-1);
	}
	if(using_h){
		printf("Hello World!\n");
	}
	if(using_f){
		printf("Printing file %s\n",filename);
		FILE *f = fopen(filename, "r");
		if(f == NULL){
			printf("Cannot open file \n");
			exit(-1);
		}
		char c;
		while((c = fgetc(f))!=EOF){
			printf("%c",c);
		}
		fclose(f);
	}
	if(argc == 2){
		pid_to_kill = atoi(argv[1]);
	}else if(argc == 3){
		pid_to_kill = atoi(argv[2]);
	}else if(argc == 5){
		pid_to_kill = atoi(argv[4]);
	}

	printf("my_kill pid: %d\n", getpid());
       	int status = kill(pid_to_kill, SIGINT);  
      	int errnum = errno;  
	if (status == -1) {      
		  fprintf(stderr, "Value of errno: %d\n", errno);      
		  perror("Error printed by perror");      
		  fprintf(stderr, "Error killing process: %s\n", strerror( errnum));
	}
	return 0;



}

