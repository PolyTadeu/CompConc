#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<math.h>
#include <string.h>


//variáveis globais
long long int N=0; 
int nthreads; 
double *numAleatoriosX;
double *numAleatoriosY;
pthread_t *tid;

//thread que faz a verificação dos pontos sob a elipse
void * elipse(void *arg) {
    long int id = (long int) arg;
    long int tamBloco = N/nthreads;  
    long int ini = id * tamBloco; 
    long int fim;
    if(id == nthreads-1) fim = N;
    else fim = ini + tamBloco;
    long int *acertos ;
    acertos = (long int *) malloc(sizeof(long int));
    int i;
    for(i = ini; i < fim; i++){
        double x = numAleatoriosX[i];
        double y = numAleatoriosY[i];
        
        if(y<=sqrt(9-9*((x*x)/4))){
           
            *acertos+=1;
        }
    }
    
    pthread_exit((void *) acertos);
}

//thread que faz a verificação dos pontos sob o semicírculo
void * semiCirculo(void *arg) {
    long int id = (long int) arg;
    long int tamBloco = N/nthreads;  
    long int ini = id * tamBloco; 
    long int fim;
    if(id == nthreads-1) fim = N;
    else fim = ini + tamBloco;
    long int *acertos ;
    acertos = (long int *) malloc(sizeof(long int));
    int i;
    for(i = ini; i < fim; i++){
        double x = numAleatoriosX[i];
        double y = numAleatoriosY[i];
        double f = sqrt(1-(x*x));
        if(y<=f){
           
            *acertos+=1;
        }
    }
    
    pthread_exit((void *) acertos);
}

//thread que faz a verificação dos pontos sob a função exponencial
void * exponencial (void *arg) {
    long int id = (long int) arg;
    long int tamBloco = N/nthreads;  
    long int ini = id * tamBloco; 
    long int fim;
    if(id == nthreads-1) fim = N;
    else fim = ini + tamBloco;
    long int *acertos ;
    acertos = (long int *) malloc(sizeof(long int));
    int i;
    for(i = ini; i < fim; i++){
        double x = numAleatoriosX[i];
        double y = numAleatoriosY[i];
        double f = (pow(x,5)/9)+7;
        if(y<=f){
           
            *acertos+=1;
        }
    }
    
    pthread_exit((void *) acertos);
}

//inicializa os vetores com coordenadas dentro do retângulo
int inicializaVetor(double b, double H){
    numAleatoriosX= (double*) malloc(sizeof(double) * N);
    if(numAleatoriosX==NULL) {
            fprintf(stderr, "ERRO--malloc\n");
        return 2;
        }
    numAleatoriosY= (double*) malloc(sizeof(double) * N);
    if(numAleatoriosY==NULL) {
            fprintf(stderr, "ERRO--malloc\n");
        return 2;
        }
    for (long int i=0; i<N; i++)
    {
        numAleatoriosX[i]= ((double) rand() / (double) RAND_MAX)*b;
        numAleatoriosY[i]= ((double) rand() / (double) RAND_MAX)*H;   
    }

    return 0;

}

//libera os vetores após o uso
void liberar(){
    
    free(tid);
    free(numAleatoriosX);
    free(numAleatoriosY);
    
}

//inicializa o vetor de indentificadores do sistema das threads
int inicializaTid()
{
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
        if(tid==NULL) {
            fprintf(stderr, "ERRO--malloc\n");
        return 2;
        }
    return 0;
}

//aguarda o fim da execução das threads 
long int esperaThreads(){
    long int acertos= 0;  
    long int *retorno;
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
            }
        acertos += *retorno;
        }
    return acertos;
}

//faz a verificação da integral de acordo com o erro permitido
void verificar(double resultado, double referncia){
    double erro= (referncia*1)/100;
    if (resultado<=referncia+erro && resultado>= referncia-erro)
        printf("Integral dentro do desvio de erro permitido!\n\n");
    else
        printf("Integral fora do desvio de erro permitido\nInsira um valor maior para N\n");
}

int main()
{
    long int acertos= 0;  
    int op=0;
    double a=0.0, b=0.0,H=0.0,ini, fim,A=0.0;
    
    //realiza o looping do menu de opções
    while (1)
    {
            
        printf("Escolha a integral que deseja calcular:\n1-Função exponencial (x⁵/9)+3 0<=x<=4\n2-Semicírculo de raio 1\n3-Elipse x²/4 + y²/9 = 1\n4-Sair\n");
        scanf("%d", &op);
        if(op==4)
            return 0;
        printf("\nDigite o número de pontos e o número de threads:");
        scanf("%Ld %d", &N,&nthreads);
        
        //redireciona para a ação correta de acordo com a opção escolhida
        switch (op)
        {
            case 1:
            {  
                ini=0;
                fim=0;
                acertos=0;
                a=0.0;
                b=4.0;
                H=(pow(b,5)/9)+7;
                A=(b-a)*H;
                inicializaVetor(b,H);
                GET_TIME(ini);
                inicializaTid();
                //criar as threads
                for(long int i=0; i<nthreads; i++) {
                if(pthread_create(tid+i, NULL, exponencial, (void*) i)){
                    fprintf(stderr, "ERRO--pthread_create\n");
                    return 3;
                    }
                }
                //aguardar o termino das threads
                acertos=esperaThreads();
                GET_TIME(fim);
                printf("\nResultados\n");
                printf("Tempo tomado pelas threads:    %lf\n", fim-ini);
                double prop= (double)acertos/N;
                printf ("Valor da integral:             %lf\n", A*prop);
                verificar(A*prop,103.85185 );
                printf("\n");
                free(tid);

            }
            break;

            case 2:
            {
                ini=0;
                fim=0;
                acertos=0;
                a=0.0;
                b=2.0;
                H=1.0;
                A=(b-a)*H;
                inicializaVetor(b,H);
                GET_TIME(ini);
                inicializaTid();
                //criar as threads
                for(long int i=0; i<nthreads; i++) {
                if(pthread_create(tid+i, NULL, semiCirculo, (void*) i)){
                    fprintf(stderr, "ERRO--pthread_create\n");
                    return 3;
                    }
                }
                //aguardar o termino das threads
                acertos=esperaThreads();
                GET_TIME(fim);
                printf("\nResultados\n");
                printf("Tempo tomado pelas threads:    %lf\n", fim-ini);
                double prop= (double)acertos/N;
                printf ("Valor da integral:             %lf\n", A*prop);
                verificar(A*prop,0.78539 );
                printf("\n");
                free(tid);
            }
            break;

            case 3:
            {
                ini=0;
                fim=0;
                acertos=0;
                a=0.0;
                b=4.0;
                H=3.0;
                A=(b-a)*H;
                inicializaVetor(b,H);
                GET_TIME(ini);
                inicializaTid();
                //criar as threads
                for(long int i=0; i<nthreads; i++) {
                if(pthread_create(tid+i, NULL, elipse, (void*) i)){
                    fprintf(stderr, "ERRO--pthread_create\n");
                    return 3;
                    }
                }
                //aguardar o termino das threads
                acertos=esperaThreads();
                GET_TIME(fim);
                printf("\nResultados\n");
                printf("Tempo tomado pelas threads:    %lf\n", fim-ini);
                double prop= (double)acertos/N;
                printf ("Valor da integral:             %lf\n", A*prop*2);
                verificar(A*prop*2, 4.71238*2 );
                printf("\n");
                free(tid);
            }
            break;
            
            default:
            {    printf("Opção incorreta");
                return 0;
            }
        }
    }
    return 0;
}