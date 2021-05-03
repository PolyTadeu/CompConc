#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

/* Variaveis globais */
long int *vet, *vet2;
int terminadas=0;
int bloqueadas=0;
int threads=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

//conrole das thraeds terminadas
void terminado(){
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    terminadas++;
    if(bloqueadas+terminadas>=threads){
        bloqueadas=0;
        pthread_cond_broadcast(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}


//funcao barreira
void barreira() {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    bloqueadas++;
    if (bloqueadas+terminadas<threads) { 
      //ultima thread a chegar na barreira
      pthread_cond_wait(&x_cond, &x_mutex);
      
    } else {
      bloqueadas=0;
      pthread_cond_broadcast(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}



//funcao das threads
void *tarefa (void *arg) {
    int id = * (int*) arg;
    long int aux=0;
    for(int i=1;i<=id; i=i*2){
        aux=vet[id-i];
        
        barreira();
        vet[id]+=aux;
        barreira();
    }

    terminado();
    

    pthread_exit(NULL);
}


//verifica resultado
void verificar(int N)
{
    for(int i=1; i<N; i++)
        vet2[i] = vet2[i] + vet2[i-1];

    for(int i=0; i<N; i++){
        if (vet2[i]!=vet[i]){
            printf("Erro\n");
            exit(0);
        }
    }

    printf("Resultado correto\n");
}


int pot (int p)
{
    int result=1;
    for (int i=0; i<p; i++)
        result= result*2;
    
    return result;
}

int main(int argc, char *argv[]) {
    
    pthread_t *tid;
    int p=0;
    int *id;

   if(argc < 2) {
       fprintf(stderr, "Digite: %s <valor da potência p>\n", argv[0]);
       return 1; 
   }
    p = atoi(argv[1]);
    
    threads=pot(p);

    if(threads>300)
        threads=256;
    
    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);


    //inicializa vetores
    vet= (long int*) malloc(sizeof(long int ) * threads);
    if(vet==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
    return 2;
    }
    vet2= (long int*) malloc(sizeof(long int ) * threads);
    if(vet2==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
    return 2;
    }

    id= (int*) malloc(sizeof(int ) * threads);
    if(id==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
    return 2;
    }

    tid = (pthread_t *) malloc(sizeof(pthread_t) * threads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
    return 2;
    }

    /* Preenche os vetores*/
    for(int i=0; i<threads; i++)
    {
        vet[i]=i+1;
        vet2[i]=i+1;
    }

    /* Cria as threads */
    for(int i=0;i<threads;i++) {
        id[i]=i;
        pthread_create(tid+i, NULL, tarefa, (void*) &id[i]);
    }

    /* Espera todas as threads completarem */
    for (int i = 0; i < threads; i++) {
        pthread_join(*(tid+i), NULL);
    }
 
    /* Verifica acorretudo do resultado*/
    verificar(threads);

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    free(tid);
    free(id);
    free(vet);
    free(vet2);


    return 0;
}
