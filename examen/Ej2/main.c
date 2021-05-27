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
    printf("%d\n", fd);
    //Saco mi pid
    pid_t pid;
    pid = gettid();
    
    //SECCION CRITICA INICIO
    pthread_mutex_lock(&posi);
        //calculo mi posicion en el fichero

    pthread_mutex_unlock(&posi);
    //SECCION CRITICA FIN

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

    //APERTURA DEL FICHERO
    //Lo abro aqui ya que la tabla de descriptores 
    // abiertos es compartida por todos los hilos del proceso
	int fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0700);


    /* CREACION DE LOS HILOS */
    for(int i = 0; i < numHilos; i++){
        pthread_create(&thread[i], NULL, func, (void*)fd);
    }
    printf("El thread principal continua ejecutando\n");

    /* FINALIZACION DE LOS HILOS */
    for(int i = 0; i < numHilos; i++){
        pthread_join(thread[i], NULL);  
    }

    //CIERRO EL ARCHIVO
    close(fd);

    //Muestro el contenido del fichero



  return 0;

}