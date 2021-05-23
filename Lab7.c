#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  4

/* Variaveis globais */
sem_t condt2, condt3;

/* Thread A */
void *D (void *t) {
  int boba1, boba2;

  
    /* faz alguma coisa pra gastar tempo... */
    boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
    printf("Seja bem-vindo!\n");
    sem_post(&condt2);
    sem_post(&condt2);
    pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {

    sem_wait(&condt2);
    printf("Fique a vontade.\n");
    sem_post(&condt3);
    pthread_exit(NULL);
}

void *C (void *t) {

    sem_wait(&condt2);
    printf("Sente-se por favor.\n");
    sem_post(&condt3);
    pthread_exit(NULL);
}

void *A (void *t) {
  sem_wait(&condt3);
  sem_wait(&condt3);
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t threads[NTHREADS];

    /* Inicia o semafaro */
    sem_init(&condt2, 0, 0);
    sem_init(&condt3, 0, 0);

    /* Cria as threads */
    pthread_create(&threads[2], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);
    pthread_create(&threads[0], NULL, C, NULL);
    pthread_create(&threads[3], NULL, D, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }


}