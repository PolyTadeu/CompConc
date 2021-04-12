//Calculo de pi
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<math.h>

long long int N; 
int nthreads; 

//fluxo das threads
void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   double *somaLocal; //variavel local da soma de elementos
   somaLocal = (double*) malloc(sizeof(double));
   if(somaLocal==NULL) {exit(1);}
   long int tamBloco = N/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = N;
   else fim = ini + tamBloco; //trata o resto se houver
   //soma os elementos do bloco da thread
   for(long int i=ini; i<fim; i++){ 
        if(i%2==0)
            *somaLocal+=(1.0/((2*i)+1));
        else
            *somaLocal-=(1.0/((2*i)+1));
    }    
   //retorna o resultado da soma local
   pthread_exit((void *) somaLocal); 
}

//fluxo principal
int main(int argc, char *argv[]) {
   double somaSeq= 0; //soma sequencial
   double somaConc= 0; //soma concorrente
   double ini, fim, t1; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   double *retorno; //valor de retorno das threads

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <numero de elementos> <numero threads>\n", argv[0]);
       return 1; 
   }
   N = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   
   //pi sequencial dos elementos
   GET_TIME(ini);
   for(long int i=0; i<N; i++){ 
        if(i%2==0)
            somaSeq+=(1.0/((2*i)+1));
        else
            somaSeq-=(1.0/((2*i)+1));
          
    }    
   GET_TIME(fim);
   t1=fim-ini;
   printf("Tempo sequencial:     %lf\n", fim-ini);

   //pi concorrente dos elementos
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //soma global
      somaConc += *retorno;
   }
   GET_TIME(fim);
   printf("Tempo concorrente:    %lf\n", fim-ini);
   printf("Ganho de desempenho:  %lf\n", t1/(fim-ini));

    puts("");
   //exibir os resultados
   printf("Constante M_PI: %.15lf\n", M_PI);
   printf("Pi sequencial:  %.15lf\n", 4*somaSeq);
   printf("Pi concorrente: %.15lf\n", 4*somaConc);

   //libera as areas de memoria alocadas
   
   free(tid);

   return 0;
}