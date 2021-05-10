#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int my_system(char * comando){
	 int status,pid;

	//Creando un nuevo proceso
	pid = fork();
	switch (pid) {
		case -1:  /* error del fork() */
		return 1;
	case 0:   /* proceso hijo */
		//printf("Proceso hijo\n");
		execlp("/bin/bash","/bin/bash", "-c", comando, (char *)NULL);
		printf("ERROR en el exec\n");
		perror("exec");
		return 2;
	default:  /* padre */
		wait(&status); //Espero a que mi hijo acabe
		//printf("Proceso padre\n");
  	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	my_system(argv[1]);
	
	//printf("Saliendo del main\n");
	return 0;
}

