#include <stdlib.h>
#include <stdio.h>

#define N_PARADAS 5 // numero de paradas de la ruta
#define EN_RUTA 0 // autobus en ruta 
#define EN_PARADA 1 // autobus en la parada 
#define MAX_USUARIOS 40 // capacidad del autobus 
#define USUARIOS 4 // numero de usuarios
//--------------- INICIO VARIABLES COMPARTIDAS -----------------------//
// estado inicial 
int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus 
int n_ocupantes = 0; // ocupantes que tiene el autobus

// personas que desean subir en cada parada --> array
int esperando_parada[N_PARADAS]; //= {0,0,...0};

// personas que desean bajar en cada parada --> array
int esperando_bajar[N_PARADAS]; //= {0,0,...0};

//--------------- FINAL VARIABLES COMPARTIDAS -----------------------//

// Otras definiciones globales (comunicacion y sincronizacion)
pthread_mutex_t mutex; //Mutex para las variables compartidas
pthread_cond_t espe_par; //Personas esperando en parada
pthread_cond_t esper_suban; //El autobus en espera a que suban todos


//Ejemplo: distintos MUTEX y VARIABLES DE CONDICION

void * thread_autobus(void * args) { 
	while (1) { //TODO ver condicion --> Puede ser infinito, la ruta es circular
		// esperar a que los viajeros suban y bajen 
		Autobus_En_Parada(); 
		// conducir hasta siguiente parada 
		Conducir_Hasta_Siguiente_Parada(); 
	}
}


void * thread_usuario(void * arg) { 
	int id_usuario;
	int a,b;
	// obtener el id del usario 
	while (1) { //TODO ver condicion
			a=rand() % N_PARADAS; //Parada de origen
		do{
			b=rand() % N_PARADAS; //parada de destino
		} while(a == b); //Para que el destino no sea el mismo que el origen

		Usuario(id_usuario,a,b);
	}
}

void Usuario(int id_usuario, int origen, int destino) {
	// Esperar a que el autobus este en parada origen para subir 
	Subir_Autobus(id_usuario, origen); 
	// Bajarme en estacion destino 
	Bajar_Autobus(id_usuario, destino);
}

//FUNCIONES AUXILIARES A IMPLEMENTAR

void Autobus_En_Parada(){ 
	/* Ajustar el estado y bloquear al autobus hasta que no haya pasajeros que 
	quieran bajar y/o subir la parada actual. Despues se pone en marcha */

	//INICIO SECCION CRITICA
	pthread_mutex_lock(&mutex);
		estado = EN_PARADA;
	pthread_mutex_unlock(&mutex);
	//FINAL SECCION CRITICA

	while(esperando_parada[parada_actual] > 0){ //Mientras haya gente que quiera subir
		cond_broadcast(&espe_par); //Aviso (yo como autobus) a todos los que estan esperando en la parada
									// para que intenten subir al autobus
		cond_wait(&esper_suban, &mutex);	//Espero (yo como autobus) a que me digan que ya subio el ultimo
	}
	//Aqui ya subieron todos

	//INICIO SECCION CRITICA
	pthread_mutex_lock(&mutex);
		estado = EN_RUTA;
	pthread_mutex_unlock(&mutex);	
	//FINAL SECCION CRITICA
}

void Conducir_Hasta_Siguiente_Parada(){ 
	/* Establecer un Retardo que simule el trayecto y actualizar numero de parada */ 
	//Generar numero aleatorio para un sleep

	//INICIO SECCION CRITICA
	pthread_mutex_lock(&mutex);
		estado = EN_RUTA;
	pthread_mutex_unlock(&mutex);	
	//FINAL SECCION CRITICA


	sleep(2/*tiempo_random*/); //CONDUCIENDO


	//Llego a la siguiente parada
	//INICIO SECCION CRITICA
	pthread_mutex_lock(&mutex);
		if(parada_actual < N_PARADAS -1)// Paradas 0 1 2 3 
			parada_actual +=1;
		else { //Esta en la ultima parada - Parada 4
			parada_actual = 0;
		}
	pthread_mutex_unlock(&mutex);	
	//FINAL SECCION CRITICA


	//INICIO SECCION CRITICA
	pthread_mutex_lock(&mutex);
		estado = EN_PARADA;
	pthread_mutex_unlock(&mutex);
	//FINAL SECCION CRITICA

}

void Subir_Autobus(int id_usuario, int origen){ 
	/* El usuario indicara que quiere subir en la parada ’origen’, esperara a que 
	el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para 
	proporcionar informacion de depuracion */ 

	//INICIO SECCION CRITICA
	esperando_parada[origen]++;
	//FINAL SECCION CRITICA

	while((estado)!= EN_PARADA
			|| (parada_actual) != origen){

		condWait( , );
		//SOLO saldra del wait si esta el autobus en su parada
	}
	// ----------------------------
	//INICIO SECCION CRITICA
	esperando_parada[origen]--;
	//FINAL SECCION CRITICA

	if(){ //SI SOY EL ULTIMO
		signal(		); //Aviso al autobus para que deje de esperar en la parada
	}
}

void Bajar_Autobus(int id_usuario, int destino){ 
	/* El usuario indicara que quiere bajar en la parada ’destino’, esperara a que 
	el autobus se pare en dicha parada y bajara. El id_usuario puede utilizarse para 
	proporcionar informacion de depuracion */ 


}

int main(int argc, char* argv[])
{
	/* ... To be completed ... */
	int i; 
	// Definicion de variables locales a main 
	pthread_t th_autobus;
	pthread_t th_usuarios[USUARIOS];
	// Opcional: obtener de los argumentos del programa la capacidad del 
	// autobus, el numero de usuarios y el numero de paradas



	// Crear el thread Autobus
	pthread_create(&th_autobus, NULL, thread_autobus, NULL);

	for (i = 0; i < USUARIOS; i++){
		// Crear thread para el usuario i
		pthread_create(&th_usuarios[i], NULL, thread_usuario, NULL);
		printf("Creando hilo de un usuario\n");
	}
	for (i = 0; i < USUARIOS; i++){ 
		// ESPERAR terminacion de los hilos de los usuarios
		pthread_join(th_usuarios[i], NULL);
		printf("Esperando terminacion del hilo de un usuario\n");
	}
	//ESPERAR al hilo del autobus
	pthread_join(th_autobus, NULL);


	exit(0);
}

