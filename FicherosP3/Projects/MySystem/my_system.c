#include <stdlib.h>
#include <stdio.h>

int my_system( char * comando){

	//Creando un nuevo proceso


}




int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	return system(argv[1]);
}

