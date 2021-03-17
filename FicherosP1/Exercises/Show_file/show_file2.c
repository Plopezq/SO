#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	char* buffer;	
	int c,ret; //Ret es para gestionar los errores

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file for reading --okay*/
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);


	while (!feof(file)) {
		/* Read file byte by byte */
		//Kei datos, cada uno de 1 byte
		int cantidad = fread(buffer, sizeof(buffer),1, file);		
		if (ferror(file)!=0)
		{
			printf("Ha ocurrido algún error en la lectura de números.\n");
		}
		// Escribo tantos como haya leido
		fwrite(buffer, 1, 1, stdout);
	}
	
	return 0;
}
