/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int dim;
float *mat1; //matriz de entrada
float *mat2;
float *saida; //vetor de saida
int nthreads; //numero de threads

//funcao que as threads executarao
void * tarefa(void *arg) {
   int idThread = * (int*) arg;
   //printf("Thread %d\n", args->id);
   for(int i=idThread; i<dim; i+=nthreads){
         for(int j=0; j<dim; j++){
            for (int k=0; k<dim; k++){
               saida[i*dim+j]+= mat1[i*dim+k]* mat2[k*dim+j];
            }
         }
      }
   pthread_exit(NULL);
}

void verifica ()
{
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         if (saida[i*dim+j]!=(dim*2))
            {
               printf("Matriz de saída incorreta\n");
               return;
            }
      }
   }
   printf("Matriz de saída correta\n");
}

//fluxo principal
int main(int argc, char* argv[]) {
   int id [nthreads];
   pthread_t *tid; //identificadores das threads no sistema
   double inicio, fim, delta;
   
   GET_TIME(inicio);
   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   mat1 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat2 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   saida = (float *) malloc(sizeof(float) * dim * dim);
   if (saida == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++)
      {
         mat1[i*dim+j] = 1;
         mat2[i*dim+j] = 2;
         saida[i*dim+j] = 0; 
      }  
   }
   GET_TIME(fim);
   delta = fim - inicio;
   printf("Tempo inicializacao:%lf\n", delta);

   //multiplicacao da matriz pelo vetor
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
  
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      id[i]=i;
      if(pthread_create(tid+i, NULL, tarefa, (void*) &id[i])){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo multiplicacao:%lf\n", delta);

   //exibicao dos resultados
   // puts("Vetor de saida:");
   // for(int i=0; i<dim; i++) {
   //    for(int j=0; j<dim; j++){
   //    printf("%.1f ", saida[i*dim+j]);
   //    }
   // }
   // puts("");
   GET_TIME(inicio);
   //liberacao da memoria e verificacao do resultado
   verifica();
   free(mat1);
   free(mat2);
   free(saida);
   free(tid);
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo finalizacao:%lf\n", delta);

   return 0;
}