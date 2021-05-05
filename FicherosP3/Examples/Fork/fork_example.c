#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define N 3

//#define EXERCISE_OPEN = true;
//VARIABLES GLOBALES
int a_global=0;
char tab[2*N]="\0";

int main() {
	//VARIABLES LOCALES
	pid_t pid;
	int b_local = 0;
	int * p_heap; //VARIABLE EN MEM DINAMICA
	int fd;
	int i;
	FILE* file_desc;
	
	p_heap = (int*) malloc (sizeof(int));
	*p_heap = 0;
	
	fd=open("file.txt",O_CREAT | O_RDWR | O_TRUNC,0666);
	file_desc = fdopen(fd,"r+");
	
	setvbuf(file_desc,(char*)NULL,_IOLBF, 0);
	
	for (i=0;i<N;i++) {
		if ( (pid=fork()) == -1) {
			printf("ERROR. Process %u could not fork. Exit\n",pid);
			exit(-1);
		}
		else if (pid==0) {
			//PROCESO HIJO
			//sleep(5); //Para que sus padres mueran e init adopte a los procesos hijos
#ifdef EXERCISE_OPEN
			fd=open("file.txt",O_CREAT | O_RDWR ,0666);
			file_desc = fdopen(fd,"r+");
			setvbuf(file_desc,(char*)NULL,_IOLBF, 0);
#endif
			strcat(tab,"\t\0");//Concatena 2 strings
			//MODIFICACION DE VARIABLES
			a_global+=2; b_local+=2; (*p_heap) +=2;
			fprintf(file_desc,"%s Child. pid=%u ppid=%u. i=%d a=%d b=%d *p=%d\n",tab,getpid(),getppid(),i,a_global,b_local,*p_heap);
			fflush(NULL);

		}
		else {
			// PROCESO PADRE
			//sleep(5); Dormimos al padre para que haya hijos zombies
			a_global++; b_local++; (*p_heap)++;
			fprintf(file_desc,"%s Parent. pid=%u ppid=%u. i=%d a=%d b=%d *p=%d\n",tab,getpid(),getppid(),i,a_global,b_local,*p_heap);
			fflush(NULL);
		}
	}
	fclose(file_desc);

	while (wait(NULL) != -1) ; //Para que sus padres mueran e init adopte a los procesos hijos
	if (errno != ECHILD) {
		printf("ERROR when waiting for children to finish\n");
		exit(-1);
	}

	exit(0);

}
