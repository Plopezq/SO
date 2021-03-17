#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 * 
 * Se usa para CREAR el TAR
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes) 
{//Suponemos que los ficheros origen y destino ya están abiertos, además tampoco los cerraremos
int c, aux;
int bytesLeidos = 0;
	/* LEEMOS byte a byte del fichero origen hasta que se llegue al final del fichero o a nBytes */
	c = getc(origin);
	while ( c != -1 && (bytesLeidos < nBytes)) {
		bytesLeidos = bytesLeidos + 1; //La funcion getc lee byte a byte (caracter a caracter)
		/* ESCRIBIMOS byte a byte en el fichero destino */
		aux = putc((unsigned char) c, destination);
		c = getc(origin);//Siguiente byte
	}
	if(aux==EOF){ //Ha fallado el getc() o el putc()
		return -1;
	}else{
		return bytesLeidos;
	}
}

/** Loads a string from a file.
 * 
 * Se usa para leer el TAR
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Consideramos que el archivo ya esta abierto
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	//TODO gestionar errores
	int cont = 0; //Contador del numero de bytes para luego poder hacer fread

	/* Leo byte a byte */
	int c; //Almaceno lo que va leyendo para depurar, pero luego no sera necesario
	while ((c = getc(file)) != '\0') { //El /0 lo mete el tar
		cont++; //Me indica los bytes que voy leyendo
	}
	cont++; //Por el \0 que ha leido el get, auqneu no entre en el while
	//Ahora que ya se el numero de bytes a leer, situo el puntero de lectura del archivo
	//al comienzo de la linea otra vez, pasando el numero de bytes que he leido anteriormente de forma negativa
	fseek(file, -cont, SEEK_CUR); //SEEK_CUR indica que se desplace desde la posicion actual


	//Aqui hago el malloc de tamaño contador y ya uso un fread()
	//Leo la cadena y la almaceno en el heap
	char * ptr = malloc(cont);
	int leido = fread(ptr,sizeof(unsigned char), cont ,file);
	if (leido < cont){ //No se ha podido leer todo, ERROR
		return NULL;
	}

	//Devolvemos la direccion de comienzo de memoria donde se encuentra la cadena de caracteres
	return ptr; //Esta memoria se liberara fuera de la funcion, despues de usar su puntero
}

/** Read tarball header and store it in memory.
 *
 * Se usa para LEER el TAR, la cebecera del TAR exactamente
 * 
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles/*Output*/)
{
	stHeaderEntry* array=NULL; 
	int nr_files = 0;

	/* ... Read the number of files (N) from tarfile and 
	store it in nr_files ... */
	int err = fread(&nr_files, sizeof(nr_files), 1, tarFile);
	if(err < 1){//Ha habido un error
		return NULL;
	}
	/* Allocate memory for the array */ 
	array = (stHeaderEntry*)malloc(sizeof(stHeaderEntry)*nr_files);

	/*... Read the (pathname ,size) pairs from tarFile and
	store them in the array ...*/
	char* aux; //puntero auxiliar para almacenar la direccion que me devolvera loadStr
	for(int i = 0; i < nr_files; i++){//Tantas veces como numeros de archivos tengo
		aux = loadstr(tarFile); //LEO el nombre del archivo
			array[i].name = aux; //Hago que el array apunte a donde esta el nombre del archivo -> ALMACENO
		//free(aux); //Libero la memoria que habia reservado la funcion loadStr en su interior
		int tam; //variable auxiliar para almacenar el tamanyo de cada archivo
		//LEO su tamaño y lo almaceno - el tamaño son 4 bytes,  tengo que leer con fread
		err = fread(&tam,sizeof(unsigned int), 1, tarFile);
		array[i].size = (unsigned int) tam;
		if(err < 1 || aux == NULL){ //Ha habido fallo al leer algo
			return NULL;
		}
	}
	/* Store the number of files in the output parameter */ 
	*nFiles = nr_files;
	return array; //Luego lo liberalemos en la funcion de fuera
}

/** Creates a tarball archive
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
    FILE* fileInput = NULL;
	FILE* fileTar = NULL;

	//Abrimos el ﬁchero mtar para escritura (ﬁchero destino)
	if ((fileTar = fopen(tarName, "w")) == NULL)
		return EXIT_FAILURE;
	//Reservamos memoria (malloc()) para un array de stHeaderEntry
		//El array tendra tantas posiciones como ﬁcheros haya que introducir en el mtar
	stHeaderEntry* array=NULL;
	array = (stHeaderEntry*)malloc(sizeof(stHeaderEntry)*nFiles);
		//Exige reservar memoria para alojar la cadena asociada a cada nombre de ﬁchero
		// (no olvidar reservar espacio para el ’ \ 0’)
	int acum = 0;
	for(int i = 0; i < nFiles; i++ ){
		int len = strlen(fileNames[i]);
		array[i].name = malloc(len + 1); //Reservo espacio
		array[i].name = fileNames[i]; //Guardo el nombre del archivo 
		acum = acum + len + 1; //Para cuando tenga que obtener el offData
	}
	//Nos posicionamos en el byte del ﬁchero donde comienza la region de datos:
		//De este modo dejamos hueco para el numero de ﬁcheros y los metadatos de cada uno (ruta,tamano)
	int offData = sizeof(int) + nFiles * sizeof(unsigned int) + acum;
	int err = fseek(fileTar, offData, SEEK_SET); //SEEK_SET indica que se desplace desde el principio
		if(err != 0) return EXIT_FAILURE; //Da error el fseek
	
	//Por cada ﬁchero (inputFile) que haya que copiar en el mtar:

	for(int j = 0; j < nFiles; j++){
		//– Abrimos inputFile para lectura
		if ((fileInput = fopen(fileNames[j], "r")) == NULL)
			return EXIT_FAILURE;
		//– copynFile(inputFile,tarFile,INT MAX)
		unsigned int bytesLeidos = copynFile(fileInput, fileTar, INT_MAX);
		//– Cerramos inputFile
		err = fclose(fileInput); //Ha dado error el cerrar el archivo
			if(err != 0) return EXIT_FAILURE;
		//– Rellenamos el elemento correspondiente del array de estructuras 
			//-con el tamano del ﬁchero que acabamos de volcar a disco
		array[j].size = bytesLeidos; //Relenamos el tamaño del archivo en el array de estructuras
	}
	//Nos posicionamos para escribir en el byte 0 del ﬁchero tar para:
	err = fseek(fileTar, 0, SEEK_SET); //SEEK_SET indica que se desplace desde el principio
		if(err != 0) return EXIT_FAILURE; 
	//– escribir numero de ﬁcheros en el ﬁchero (4 bytes)
	err = fwrite(&nFiles,sizeof(int),1,fileTar);
		if(err < 1) return EXIT_FAILURE;
	//– Para cada estructura stHeaderEntry:
	for(int z = 0; z < nFiles; z++){
		//escribir la ruta del ﬁchero (con ’\0’ al ﬁnal)
		int aux = 0;
		while (aux <= sizeof(array[z].name)) {
			/* Escribimos byte a byte el nombre del fichero */
			err = putc((unsigned char) array[z].name[aux], fileTar);
			if(err == EOF) return EXIT_FAILURE;
			aux++;
		}
			//Escribo el \0 al final
			char aux2 = '\0';
			err = putc((unsigned char) aux2, fileTar);
			if(err == EOF) return EXIT_FAILURE;
			//Escribimos el numero de bytes que ocupa el fichero
			int bytes = array[z].size;
			err = fwrite(&bytes,sizeof(unsigned int),1,fileTar);
			//err = putc((unsigned char) bytes, fileTar);
			if(err == EOF) return EXIT_FAILURE;
	}
	// Liberamos memoria y cerramos el fichero mtar
	//free(array->name);
	free(array);
	fclose(fileTar);
		if(err != 0) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{
	FILE* fileTar = NULL;
	int nFiles = 0; //Numero de archivos a extraer

	//Abrimos el ﬁchero mtar para lectura (ﬁchero destino)
	if ((fileTar = fopen(tarName, "r")) == NULL)
		return EXIT_FAILURE;

	//Llamo a la funcion que lee
	stHeaderEntry* array=NULL;
	array = readHeader(fileTar, &nFiles);

	//Una vez leido el header, extraigo los archivos
	for(int i = 0; i < nFiles; i++){
		//Creo un archivo para escritura con el nombre proporcionado
		FILE* fileOut = NULL;
		//Abrimos el ﬁchero mtar para escritura (ﬁchero destino)
		if ((fileOut = fopen(array[i].name, "w+")) == NULL)
			return EXIT_FAILURE;
		//voy leyendo byte a byte hasta su tamaño
		//int auxiliar = array[i].size; //Con esta variable auxiliar, debuggeara mujer
		for(int j = 0; j < array[i].size; j++){
			//Leo byte a byte del tarFile y creo su archivo -> fileOut
			int c = getc(fileTar);
			/* Print byte to fileOut */
			int ret=putc((unsigned char) c, fileOut);
			if (ret==EOF || c == EOF){ //fallo al leer o escribir
				fclose(fileOut);
				return EXIT_FAILURE;
			}
		}
		//cierro el archivo
		fclose(fileOut);
	}
	free(array);
	return EXIT_SUCCESS;
}
