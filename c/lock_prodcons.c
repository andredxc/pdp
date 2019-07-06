#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define PROD_STRING "1"
#define PROD_STRING_REPS 5000

int NUM_THREADS = 1;
char *PRODUCT;
char* _production;
pthread_mutex_t *_locks;


void *consumer(void *x);
void *producer(void *x);


int main(int argc, char** argv){

    int i;
    pthread_t producerThreads[NUM_THREADS];
    pthread_t consumerThreads[NUM_THREADS];
    int threadIndexes[NUM_THREADS];
    struct timeval stop, start;

    // Inicializa a string de producao
    PRODUCT = (char*) malloc(PROD_STRING_REPS*strlen(PROD_STRING)*sizeof(char));
    PRODUCT[0] = '\0';
    for(i = 0; i < PROD_STRING_REPS; i++){
        strcat(PRODUCT, PROD_STRING);
    }

    // Inicializa o vetor de producao
    _production = (char*) malloc(NUM_THREADS*sizeof(char));
    for(i = 0; i < NUM_THREADS; i++){
        _production[i] = '0';
    }

    // Inicia captura do tempo
    gettimeofday(&start, NULL);

     // Cria threads de produtores e consumidores e locks
     _locks = (pthread_mutex_t*) malloc(NUM_THREADS*sizeof(pthread_mutex_t));
    for(i = 0; i < NUM_THREADS; i++){
        threadIndexes[i] = i;
        pthread_create(&producerThreads[i], NULL, producer, &threadIndexes[i]);
        pthread_create(&consumerThreads[i], NULL, consumer, &threadIndexes[i]);
    }

    // Espera ate o fim da execucao de todas as threads
    for(i = 0; i < NUM_THREADS; i++){
        pthread_join(producerThreads[i], NULL);
        pthread_join(consumerThreads[i], NULL);
    }

    // Finaliza captura do tempo
    gettimeofday(&stop, NULL);
    printf("Tempo de execucao: %.3f ms\n", (stop.tv_usec - start.tv_usec)/1000.0);
    return 1;
 }


 void *consumer(void *x){

     char *readBuffer = (char*) malloc(sizeof(PRODUCT));
     int *indexPtr, index, cont;

     indexPtr = (int*) x;
     index = *indexPtr;

     cont = 0;
     while(cont < strlen(PRODUCT)){
         // Pega o lock para fazer alteracoes no vetor de producao
         pthread_mutex_lock(&_locks[index]);
         if(_production[index] != '0'){
             // Le e atualiza o buffer
             readBuffer[cont] = _production[index];
             _production[index] = '0';
             cont++;
         }
         // Libera o lock
         pthread_mutex_unlock(&_locks[index]);
     }

     // Verifica se a operacao deu certo
     if(strcmp(readBuffer, PRODUCT) != 0){
         fprintf(stderr, "Consumer %d - FAILURE \'%s\'\n", index, readBuffer);
     }

     return NULL;
 }


 void *producer(void *x){
     char *writeBuffer = (char*) malloc(sizeof(PRODUCT));
     int *indexPtr, index, cont;

     indexPtr = (int*) x;
     index = *indexPtr;

     cont = 0;
     while(cont < strlen(PRODUCT)){
         // Pega o lock para fazer alteracoes no vetor de producao
         pthread_mutex_lock(&_locks[index]);
         if(_production[index] == '0'){
             // Le e atualiza o buffer
             writeBuffer[cont] = PRODUCT[cont];
             _production[index] = PRODUCT[cont];
             cont++;
         }
         // Libera o lock
         pthread_mutex_unlock(&_locks[index]);
     }

     // Verifica se a operacao deu certo
     if(strcmp(writeBuffer, PRODUCT) != 0){
         fprintf(stderr, "Producer %d - FAILURE \'%s\'\n", index, writeBuffer);
     }

     return NULL;
 }
