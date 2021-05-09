#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define N_PARADAS 5 // numero de paradas de la ruta
#define EN_RUTA 0 // autobus en ruta 
#define EN_PARADA 1 // autobus en la parada 
#define MAX_USUARIOS 40 // capacidad del autobus 
#define USUARIOS 4 // numero de usuarios
//--------------- INICIO VARIABLES COMPARTIDAS -----------------------//
// estado inicial 
int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus 
int n_ocupantes = 4; // ocupantes que tiene el autobus

// personas que desean subir en cada parada --> array
int esperando_parada[N_PARADAS]; //= {0,0,...0};

// personas que desean bajar en cada parada --> array
int esperando_bajar[N_PARADAS]; //= {0,0,...0};

//--------------- FINAL VARIABLES COMPARTIDAS -----------------------//

// Otras definiciones globales (comunicacion y sincronizacion)
pthread_mutex_t mutex; //Mutex para las variables compartidas
pthread_cond_t espe_par; //Personas esperando a subir o bajar en una parada
pthread_cond_t esper_suban; //El autobus en espera a que suban todos
pthread_cond_t esper_bajar; //El autobus espera a que bajen todos

//Ejemplo: distintos MUTEX y VARIABLES DE CONDICION



//FUNCIONES AUXILIARES A IMPLEMENTAR

void Autobus_En_Parada(){ 
	/* Ajustar el estado y bloquear al autobus hasta que no haya pasajeros que 
	quieran bajar y/o subir la parada actual. Despues se pone en marcha */
	printf("Autobus llega a la parada %d \n", parada_actual);

	//INICIO SECCION CRITICA
	pthread_mutex_lock(&mutex);
		estado = EN_PARADA;
	//pthread_mutex_unlock(&mutex);
	//FINAL SECCION CRITICA
	//TODO ver si hace falta una seccion critica
	while(esperando_parada[parada_actual]){ //Mientras haya gente que quiera subir
		pthread_cond_broadcast(&espe_par); //Aviso (yo como autobus) a todos los que estan esperando en la parada
									// para que intenten subir al autobus
		if(esperando_bajar[parada_actual] > 0){ //si alguien quiere bajar en esta parada
			printf("autobus esperando a que bajentodos\n");
			pthread_cond_wait(&esper_bajar, &mutex);	//Espero (yo como autobus) a que me digan que ya bajo el ultimo
		} 
		if(esperando_parada[parada_actual] > 0){ //si alguien quiere subir en esta parada
			printf("autobus esperando a que suban todos\n");
			pthread_cond_wait(&esper_suban, &mutex);	//Espero (yo como autobus) a que me digan que ya subio el ultimo
		}
		
	}
	//Aqui ya subieron y bajaron todos los de esta parada

	//INICIO SECCION CRITICA
	//pthread_mutex_lock(&mutex);
		estado = EN_RUTA;
	pthread_mutex_unlock(&mutex);	
	//FINAL SECCION CRITICA
	printf("Autobus se va de la parada %d \n", parada_actual);

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
		estado = EN_PARADA;
	pthread_mutex_unlock(&mutex);
	//FINAL SECCION CRITICA
	printf("Autobus en marcha \n");

}

void Subir_Autobus(int id_usuario, int origen){ 
	/* El usuario indicara que quiere subir en la parada ’origen’, esperara a que 
	el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para 
	proporcionar informacion de depuracion */ 
	// SOLO pueden subir de uno en uno, asique toda
	// la funcion sera una seccion critica
	//INICIO SECCION CRITICA
	printf("Usuario %d esperando a subir en la parada %d \n", id_usuario, origen);

	pthread_mutex_lock(&mutex);
		esperando_parada[origen]++; //Una persona mas a subir en esa parada

		while((estado)!= EN_PARADA
				|| (parada_actual) != origen){
			pthread_cond_wait(&espe_par, &mutex); //Me bloqueo hasta que el bus me avise de que esta en una parada
			//PUEDE que la parada no sea la mia, por eso esta el while
			//SOLO saldra del wait si esta el autobus en su parada
		}
		esperando_parada[origen]--; //La persona se sube al autobus
		printf("Ya me subi al autobus");
		if(esperando_parada[parada_actual] == 0){ //SI SOY EL ULTIMOen subir de la parada
			printf("Ya subimos todos los de esta parada al autobus\n");
			pthread_cond_signal(&esper_suban); //Aviso al autobus para que deje de esperar en la parada
		}
	pthread_mutex_unlock(&mutex);
	//FINAL SECCION CRITICA
	printf("Usuario %d se sube al bus en la parada %d \n", id_usuario, origen);
}

void Bajar_Autobus(int id_usuario, int destino){ 
	/* El usuario indicara que quiere bajar en la parada ’destino’, esperara a que 
	el autobus se pare en dicha parada y bajara. El id_usuario puede utilizarse para 
	proporcionar informacion de depuracion */ 

	// SOLO pueden bajar de uno en uno, asique toda
	// la funcion sera una seccion critica
	//INICIO SECCION CRITICA
	printf("Usuario %d esperando a bajar en la parada %d \n", id_usuario, destino);
	pthread_mutex_lock(&mutex);
		esperando_bajar[destino]++; //Una persona mas a bajar en esa parada
		while((estado)!= EN_PARADA
				|| (parada_actual) != destino){
			pthread_cond_wait(&espe_par, &mutex); //Me bloqueo hasta que el bus me avise de que esta en una parada
			//PUEDE que la parada no sea la mia, por eso esta el while
			//SOLO saldra del wait si esta el autobus en la parada que me interesa
		}
		printf("Ya he llegado a mi parada");
		esperando_bajar[destino]--; //La persona se baja del
		n_ocupantes--;
		if(esperando_parada[parada_actual] == 0){ //Si soy el ultimo en subir de la parada
			pthread_cond_signal(&esper_bajar); //Aviso al autobus para que deje de esperar en la parada
		}
	pthread_mutex_unlock(&mutex);
	//FINAL SECCION CRITICA
	printf("Usuario %d se baja del bus en la parada %d \n", id_usuario, destino);

}
void * thread_autobus(void * args) { 
	while (n_ocupantes > 0) { //TODO ver condicion --> Puede ser infinito, la ruta es circular
		// esperar a que los viajeros suban y bajen 
		Autobus_En_Parada(); 
		// conducir hasta siguiente parada 
		Conducir_Hasta_Siguiente_Parada(); 
	}
}
void Usuario(int id_usuario, int origen, int destino) {
	// Esperar a que el autobus este en parada origen para subir 
	Subir_Autobus(id_usuario, origen); 
	// Bajarme en estacion destino 
	Bajar_Autobus(id_usuario, destino);
}

void * thread_usuario(void * arg) { 
	int *id =(int *) arg;
	int id_usuario = *id;
	int a,b;
	// obtener el id del usario 
	//while (1) { //TODO ver condicion
			a=rand() % N_PARADAS; //Parada de origen
		do{
			b=rand() % N_PARADAS; //parada de destino
		} while(a == b); //Para que el destino no sea el mismo que el origen

		Usuario(id_usuario,a,b);
	//}
}


int main(int argc, char* argv[])
{
	/* ... To be completed ... */
	int i;
	// Definicion de variables locales a main
	//Declaracion de los hilos
	pthread_t th_autobus;
	pthread_t th_usuarios[USUARIOS];
	//Declaracion del mutex y las variables condicionales
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&espe_par, NULL);
	pthread_cond_init(&esper_suban, NULL);
	pthread_cond_init(&esper_bajar, NULL);
	// Opcional: obtener de los argumentos del programa la capacidad del 
	// autobus, el numero de usuarios y el numero de paradas


	for (i = 0; i < USUARIOS; i++){
		// Crear thread para el usuario i
		printf("Creando hilo de un usuario\n");
		pthread_create(&th_usuarios[i], NULL, thread_usuario, (void*)&i );
	}
	// Crear el thread Autobus
	printf("Creando hilo del autobus\n");
	pthread_create(&th_autobus, NULL, thread_autobus, NULL);

	for (i = 0; i < USUARIOS; i++){ 
		// ESPERAR terminacion de los hilos de los usuarios
		printf("Esperando terminacion del hilo de un usuario\n");
		pthread_join(th_usuarios[i], NULL);
	}
	//ESPERAR al hilo del autobus
	pthread_join(th_autobus, NULL);

	//Destruccion de mutex y variables condicionales
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&espe_par);
	pthread_cond_destroy(&esper_suban);
	pthread_cond_destroy(&esper_bajar);



	exit(0);
}

