
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0, y=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *D (void *t) {
  int boba1, boba2;

  //printf("D: Comecei\n");
  
  /* faz alguma coisa pra gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("Seja bem-vindo!\n");

  pthread_mutex_lock(&x_mutex);
  y++;
  if (y==1) {
      //printf("D:  x = %d, vai sinalizar a condicao \n", x);
      pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
   // printf("B: Comecei\n");

    pthread_mutex_lock(&x_mutex);
    if (y==0) { 
       // printf("B: x= %d, vai se bloquear...\n", x);
        pthread_cond_wait(&x_cond, &x_mutex);
        //printf("B: sinal recebido e mutex realocado, x = %d\n", x);
  }
    printf("Fique a vontade.\n");
    x++;
    if (x==2) {
    //printf("B:  x = %d, vai sinalizar a condicao \n", x);
    pthread_cond_signal(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex); 
    pthread_exit(NULL);
}

void *C (void *t) {
  //printf("C: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  if (y==0) { 
     //printf("C: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&x_cond, &x_mutex);
     //printf("C: sinal recebido e mutex realocado, x = %d\n", x);
  }
  printf("Sente-se por favor.\n");
    x++;
    if (x==2) {
    //printf("B:  x = %d, vai sinalizar a condicao \n", x);
    pthread_cond_signal(&x_cond);
    }
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

void *A (void *t) {
  //printf("A: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  while (x < 2) { 
     //printf("A: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&x_cond, &x_mutex);
     //printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[2], NULL, A, NULL);
  pthread_create(&threads[1], NULL, B, NULL);
  pthread_create(&threads[0], NULL, C, NULL);
  pthread_create(&threads[3], NULL, D, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}