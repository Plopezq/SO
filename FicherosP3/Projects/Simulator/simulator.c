#include <stdlib.h>
#include <stdio.h>

#define N_PARADAS 5 // numero de paradas de la ruta
#define EN_RUTA 0 // autobus en ruta 
#define EN_PARADA 1 // autobus en la parada 
#define MAX_USUARIOS 40 // capacidad del autobus 
#define USUARIOS 4 // numero de usuarios
// estado inicial 
int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus 
int n_ocupantes = 0; // ocupantes que tiene el autobus

// personas que desean subir en cada parada
int esperando_parada[N_PARADAS]; //= {0,0,...0};

// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS]; //= {0,0,...0};

// Otras definiciones globales (comunicacion y sincronizacion)

void * thread_autobus(void * args) { 
	while (/* condicion */) { 
		// esperar a que los viajeros suban y bajen 
		Autobus_En_Parada(); 
		// conducir hasta siguiente parada 
		Conducir_Hasta_Siguiente_Parada(); 
	}
}

void * thread_usuario(void * arg) { 
	int id_usuario;
	// obtener el id del usario 
	while (/* condicion */) {
		a=rand() % N_PARADAS;
	do{
		b=rand() % N_PARADAS;
	} while(a==b);
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

	
}

void Conducir_Hasta_Siguiente_Parada(){ 
	/* Establecer un Retardo que simule el trayecto y actualizar numero de parada */ 


}

void Subir_Autobus(int id_usuario, int origen){ 
	/* El usuario indicara que quiere subir en la parada ’origen’, esperara a que 
	el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para 
	proporcionar informacion de depuracion */ 


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
	// Opcional: obtener de los argumentos del programa la capacidad del 
	// autobus, el numero de usuarios y el numero de paradas

	// Crear el thread Autobus

	for (i = 0; ...){ 
		// Crear thread para el usuario i

	// Esperar terminacion de los hilos return 0;
		return 0;
	}

}

