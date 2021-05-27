#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

//VARIABLES COMPARTIDAS POR TODOS LOS HILOS
int posicion;
pthread_mutex_t posi; //Para controlar el acceso a la variable compartida posicion


void * func(void * arg) 
{
    int fd = (int*)arg;
    //printf("%d\n", fd);
    //Saco mi pid
    pid_t pid;
    pid = gettid();
    char* archivo = (char *) arg;
    //APERTURA DEL FICHERO
	fd = fopen(archivo, "w+");
    //SECCION CRITICA INICIO
    pthread_mutex_lock(&posi);
        //calculo mi posicion en el fichero
        //posicion = 
    pthread_mutex_unlock(&posi);
    //SECCION CRITICA FIN

    //aHORA TOCARIA HACER UN LSEEK con el valor de posicion

    //ESCRIBIR EN EL FICHERO

    fclose(fd);
  printf("Thread x \n");
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
   
    if(argc != 3){
        printf("Uso incorrecto del programa \n");
        return -1;
    }
    int numHilos = *argv[1] - '0';
    pthread_t thread[numHilos]; //vector con el numero de hilos
    pthread_mutex_init(&posi, NULL); //inicializo el mutex


    char* archivo = argv[2];

    /* CREACION DE LOS HILOS */
    for(int i = 0; i < numHilos; i++){
        //Me quedaria pasarle pasarle tambien la i, podria crear un struct para pasarle 
        // las dos variables a la vez a func
        pthread_create(&thread[i], NULL, func, (void*)archivo);
    }
    printf("El thread principal continua ejecutando\n");

    /* FINALIZACION DE LOS HILOS */
    for(int i = 0; i < numHilos; i++){
        pthread_join(thread[i], NULL);  
    }

 

    //MUESTRO EL CONTENIDO DEL FICHERO
    int bytesLeidos;
    char contenido[1024];
    int fd = fopen(argv[2], "r+");//lo abro para solo lectura
    if (fread( &contenido, sizeof(int),1,fd) < 1) { 
            		perror("write");
			fclose(fd);
            		exit(-1);
    }
    fclose(fd);
  return 0;

}