#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <semaphore.h>

#define PROD_STRING "a"
#define PROD_STRING_REPS 5000

int NUM_THREADS = 1;
char *PRODUCT;
char* _production;
sem_t *_semaphores;


void *consumer(void *x);
void *producer(void *x);


int main(int argc, char** argv){

    int i, diff;
    struct timeb end, start;

    // Verifica parametros da linha de comando
    if(argc != 2){
        fprintf(stderr, "Usage: ./sem <NUM_THREADS>\n");
        return -1;
    }
    NUM_THREADS = atoi(argv[1]);
    if(NUM_THREADS <= 0){
        fprintf(stderr, "Usage: ./sem <NUM_THREADS>\n");
        return -1;
    }
    fprintf(stderr, "Running with %d threads\n", NUM_THREADS);
    pthread_t producerThreads[NUM_THREADS];
    pthread_t consumerThreads[NUM_THREADS];
    int threadIndexes[NUM_THREADS];

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
    ftime(&start);

     // Cria threads de produtores e consumidores e locks
     _semaphores = (sem_t*) malloc(NUM_THREADS*sizeof(sem_t));
    for(i = 0; i < NUM_THREADS; i++){
        threadIndexes[i] = i;
        sem_init(&_semaphores[i], 0, 1);
        pthread_create(&producerThreads[i], NULL, producer, &threadIndexes[i]);
        pthread_create(&consumerThreads[i], NULL, consumer, &threadIndexes[i]);
    }

    // Espera ate o fim da execucao de todas as threads
    for(i = 0; i < NUM_THREADS; i++){
        pthread_join(producerThreads[i], NULL);
        pthread_join(consumerThreads[i], NULL);
    }

    // Finaliza captura do tempo
    ftime(&end);
    diff = (int) (1000.0 * (end.time - start.time)
            + (end.millitm - start.millitm));
    fprintf(stderr, "Elapsed time: %u ms\n", diff);
    return 1;

    // Destroi os semaforos
    for(i = 0; i < NUM_THREADS; i++){
        sem_destroy(&_semaphores[i]);
    }
 }


 void *consumer(void *x){

     char *readBuffer = (char*) malloc(sizeof(PRODUCT));
     int *indexPtr, index, cont;

     indexPtr = (int*) x;
     index = *indexPtr;

     // fprintf(stderr, "Consumer %d spawned\n", index);

     cont = 0;
     while(cont < strlen(PRODUCT)){
         // Pega o lock para fazer alteracoes no vetor de producao
         sem_wait(&_semaphores[index]);
         if(_production[index] != '0'){
             // Le e atualiza o buffer
             readBuffer[cont] = _production[index];
             _production[index] = '0';
             cont++;
             // fprintf(stderr, "Consumer %d - readBuffer %s\n", index, readBuffer);
         }
         // Libera o lock
         sem_post(&_semaphores[index]);
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

     // fprintf(stderr, "Producer %d spawned\n", index);

     cont = 0;
     while(cont < strlen(PRODUCT)){
         // Pega o lock para fazer alteracoes no vetor de producao
         sem_wait(&_semaphores[index]);
         if(_production[index] == '0'){
             // Le e atualiza o buffer
             writeBuffer[cont] = PRODUCT[cont];
             _production[index] = PRODUCT[cont];
             cont++;
             // fprintf(stderr, "Producer %d - writeBuffer %s\n", index, writeBuffer);
         }
         // Libera o lock
         sem_post(&_semaphores[index]);
     }

     // Verifica se a operacao deu certo
     if(strcmp(writeBuffer, PRODUCT) != 0){
         fprintf(stderr, "Producer %d - FAILURE \'%s\'\n", index, writeBuffer);
     }

     return NULL;
 }
