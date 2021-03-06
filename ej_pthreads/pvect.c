// Producto vectorial.
// Francisco D. Igual.

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define N 10

#define IMPRIME 0

// Vectores de entrada para operar.
double * vector1;
double * vector2;

// Vector para resultado de producto secuencial.
double * vector3;

// Vector para resultado de producto paralelo.
double * vector4;

// Vector de hilos.
pthread_t * hilos;

// Tamaño de los vectores.
int tam;

// Numero de hilos.
int num_hilos;

// Vector con valor númerico (1...num_hilos) para identificar orden de hilo.
int * id_hilos;

double tiempo()
{
  // COMPLETAR. Debe retornar una marca de tiempo en milisegundos. -> hecho
	struct timeval current_time;
	gettimeofday(&current_time, NULL); //la timezone nos da igual
	double seconds = current_time.tv_sec;
	double microSeconds = current_time.tv_usec;

	return ((seconds * 1000)+(microSeconds * 0.001)); //devuelvo los milisegundos
}

int check( double * a, double *b, double n )
{
  int i = 0;
  
  for( i = 0; i < n; i++ )
  {
	  if( a[i] != b[i] )
	  {
		  return 1;
	  }
  }
  return 0;
}

void inicializa_vector( double * a, double n )
{
  int i = 0;
  
  for( i = 0; i < n; i++ )
  {
	  a[i] = rand() % 10;
  }

}

void imprime_vector( double * a, int n )
{
  int i = 0;
  
  printf( "[ " );

  for( i = 0; i < n; i++ )
  {
    printf( "%f ", a[i] );
  }

  printf( "]\n" );

}

// Punto de entrada para los hilos. 
void * pvec_trozo(void* arg)
{
  // COMPLETAR. Recibe un valor entero entre 0 y num_hilos y realiza la operación
  // sobre los elementos correspondientes del vector.
  
  int *p = (int *) arg; //Casteo el entero pasado como parametro

  int i, chunk;
  int numero_hilo = *p; //El pasado por parametro
  int ini, fin, num_valores;

  num_valores = tam / num_hilos;

  ini = num_valores * numero_hilo;
  fin = ini + num_valores;

  printf( "Thread %ld activo con id %d. Sumara %d valores, desde %d hasta %d\n", pthread_self(), numero_hilo + 1, num_valores, ini, fin );
  // COMPLETAR. Operación
	for( i = ini; i < fin; i++ )
	{
		//Accedo a los vectores de manera global, ya que abajo dice que seran compartidos por todos los hilos
		vector4[i] = vector1[i] * vector2[i]; 
	}
}

// Operacion paralela.
void pvec_paralelo( double * a, double * b, double * c, int n, int num_hilos ) //Aqui esta la violacion del segmento
{
	int i = 0;
	// COMPLETAR. Creamos cada hilo e invocamos a pvec_trozo como punto de entrada
	// Proporcionamos un identificador entre 0 y num_hilos para cada nuevo hilo. ->dentro de el primer for
	pthread_attr_t attr;
	hilos = (pthread_t*)malloc(num_hilos*sizeof(pthread_t)); //inicializo los hilos
	id_hilos = (int*)malloc(num_hilos*sizeof(int)); //inicializo el vector de id_hilos
	pthread_attr_init(&attr); //Se inicializan los atributos
	for( i = 0; i < num_hilos; i++ )
	{
		id_hilos[i] = i;
		pthread_create(&hilos[i], &attr, pvec_trozo, (void*)&id_hilos[i]);
	}
	// COMPLETAR. Esperamos a la finalización de los hilos.
	for( i = 0; i < num_hilos; i++ )
	{
		pthread_join(hilos[i], NULL);
	}
}

// Operacion secuencial.
void pvec_secuencial( double * a, double * b, double * c, int n )
{
  int i = 0;

  for( i = 0; i < n; i++ )
  {
    c[i] = a[i] * b[i];
  } 

}

// Programa principal.
int main( int argc, char * argv[] )
{
	int i;

        double tic, toc;

	if( argc != 3 )
	{
		printf( "Uso: ./pvect tamanyo num_hilos\n" );
		printf( "tamanyo debe ser multiplo del numero de hilos\n" );
		exit( -1 );
	}

	// Recogemos tamanyo de los vectores (num. de elementos).
	tam = atoi( argv[1] );

	// Recogemos numero de hilos.
	num_hilos = atoi( argv[2] );

	if( tam % num_hilos != 0 )
	{
		printf( "Tamanyo debe ser multiplo del numero de hilos\n" );
		exit( -2 );
	}

	// Vector de hilos.
	hilos = (pthread_t*) malloc( num_hilos * sizeof( pthread_t ) );

	// Vector de identificadores de hilo (0...num_hilos-1)
	id_hilos = (int*) malloc( num_hilos * sizeof( int ) );

	// Alojamos vectores en el heap. Seran compartidos por todos los hilos.
	vector1 = (double *) malloc( tam * sizeof( double ) );
	vector2 = (double *) malloc( tam * sizeof( double ) );
	vector3 = (double *) malloc( tam * sizeof( double ) );
	vector4 = (double *) malloc( tam * sizeof( double ) );

	// Inicializamos los vectores.
	inicializa_vector( vector1, tam );
	inicializa_vector( vector2, tam );
	inicializa_vector( vector3, tam );
	inicializa_vector( vector4, tam );

#if IMPRIME
	imprime_vector( vector1, tam );
	imprime_vector( vector2, tam );
	imprime_vector( vector3, tam );
	imprime_vector( vector4, tam );
#endif

	printf( "Suma secuencial\n" );
	tic = tiempo();
	//printf( "suma secuencial. TIC: %f\n", tic);
	pvec_secuencial( vector1, vector2, vector3, tam );
	toc = tiempo();
	//printf( "suma secuencial. TOC: %f\n", toc);
	printf( "Fin suma secuencial. Tiempo: %f\n", toc - tic );

#if IMPRIME
	imprime_vector( vector3, tam );
#endif

	printf( "Suma paralela\n" );
	tic = tiempo();
	pvec_paralelo( vector1, vector2, vector4, tam, num_hilos );
	toc = tiempo();
	printf( "Fin suma paralela. Tiempo: %f\n", toc - tic );

#if IMPRIME
	imprime_vector( vector4, tam );
#endif
	if( check( vector3, vector4, tam ) == 0 )
	{
		printf( "Resultado correcto!!\n" );
	}
	else
	{
		printf( "Resultado incorrecto!!\n" );
	}


	// Liberamos memoria en el heap.
	free( vector1 );
	free( vector2 );
	free( vector3 );
	free( vector4 );

}
