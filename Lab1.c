//Polyana Tadeu Pacheco da Silva

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2
#define TAM 10000

int vetor[TAM];

typedef struct {
   int inicio, fim;
} t_limite;

void *incrementa (void *arg)
{
    t_limite *args = (t_limite *) arg;

    for (int i = args->inicio; i< args->fim; i++)
           vetor[i]+=1;

    pthread_exit(NULL);
}

void verificar()
{
    for (int i=0; i<TAM; i++)   
    {
        if(vetor[i]!=4)
        {
            printf("Saída com erro! \n");
            exit(-1);
        }
    }
    printf("Saída correta! \n");
}

int main(void)
{   
    pthread_t tid[NTHREADS];
    t_limite *arg1;
    t_limite *arg2;
    int thread;

    //preenche todas as posições do vetor com o valor 3
    for (int i=0; i<TAM; i++)
        vetor[i]=3;
    
    arg1 = malloc(sizeof(t_limite));
    if (arg1 == NULL) {
      printf("Erro de alocação\n"); 
      
    }
    arg2 = malloc(sizeof(t_limite));
    if (arg2 == NULL) {
      printf("Erro de alocação\n"); 
      
    }
    arg1->inicio=0;
    arg1->fim=(TAM/2);
    arg2->inicio=(TAM/2);
    arg2->fim=TAM;

    if (pthread_create(&tid[0], NULL, incrementa, (void*) arg1)) 
    {
        printf("Erro na criação da thread\n");
        exit(-1);
        
    }
    

    if (pthread_create(&tid[1], NULL, incrementa, (void*) arg2)) 
    {
        printf("Erro na criação da thread\n");
        exit(-1);
    }


    for(thread=0; thread<NTHREADS; thread++)
    {
        if (pthread_join(tid[thread], NULL)) {
         printf("Erro ao esperar o término da execução das threads \n"); 
         exit(-1); 
        } 
    }
    
    
    verificar();
    
    
    return 0;
}