#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int total_sum = 0;
void * partial_sum(void * arg) {
  int j = 0;
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];

  for (j = ni; j <= nf; j++){
    //SECCION CRITICA
      total_sum = total_sum + j;
    //FIN DE SECCION CRITICA
  }
  pthread_exit(0);
}
//COMO ESTRUCTURAR
int main(int argc, char *argv[]) {
  int numHilos = argv[1] - '0';
  pthread_t thread[numHilos];
  
  int num1[2]={  1,   4999};
  int num2[2]={5000, 10000};

  /* CREACION DE LOS HILOS */
  for(int i = 0; i < numHilos; i++){
    pthread_create(&thread[i], NULL, partial_sum, (void*)num1);
    pthread_create(&thread[i], NULL, partial_sum, (void*)num2);
  }

  /* FINALIZACION DE LOS HILOS */
  for(int i = 0; i < numHilos; i++){
    pthread_join(thread, NULL);  
  }
  printf("total_sum=%d and it should be 50005000\n", total_sum);

  return 0;
}
