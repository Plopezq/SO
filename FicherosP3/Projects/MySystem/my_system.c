#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int my_system(const char * comando){
	pid_t pid;
	int status;
	//Creando un nuevo proceso
	pid = fork();

	switch (pid) {
		case -1:  /* error del fork() */
		return 1;
	case 0:   /* proceso hijo */
		// --execlp( comando ); /// OJO, hay que hacerlo bien
		//	execlp( /bin/bash -c comando ); /// OJO, hay que hacerlo bien
		printf("Proceso hijo\n");
  		execlp("/bin/bash","/bin/bash", "-c", "ls -l", (char *)NULL);
		perror("exec");
		return 2;
	default:  /* padre */
		wait(); //Espero a que mi hijo acabe
		printf("Proceso padre\n");
  }


	return 0;
}




int main(int argc, char* argv[])
{
	//if (argc!=2){
	//	fprintf(stderr, "Usage: %s <command>\n", argv[0]);
	//	exit(1);
	//}

	//system(argv[1]);
	my_system(argv[1]);
	
	printf("Estamos en el main\n");

	return 0;
}

