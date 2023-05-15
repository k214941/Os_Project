#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<string.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
 
sem_t mutex; // Semaphore for mutual exclusion of readcount
sem_t wblock; // Semaphore for writer exclusion
 
int x = 0; // Shared resource
 
int readcount = 0;
 
void *reader(void *arg) {
    int *t = (int *)arg; // Temporary variable t
    int f = *t;
    
    // Entry section
    sem_wait(&mutex); // Acquire mutex to update readcount
    readcount++;
    if (readcount == 1) {
        sem_wait(&wblock); // If first reader, acquire wblock to block writers
    }
    sem_post(&mutex); // Release mutex after updating readcount
    
    // Critical section (Reading shared resource)
    printf("Data read by the reader %d is %d\n", f, x);
   syscall(335,f,”Data read by the reader\n”);
  
    
    // Exit section
    sem_wait(&mutex); // Acquire mutex to update readcount
    readcount--;
    if (readcount == 0) {
        sem_post(&wblock); // If last reader, release wblock to allow writers
    }
    sem_post(&mutex); // Release mutex after updating readcount
}
 
void *writer(void *arg) {
    int *t = (int *)arg;
    int f = *t;
    
    // Entry section
    sem_wait(&wblock); // Acquire wblock to block other writers
    
    // Critical section (Writing shared resource)
    x++; // Modify the shared resource (writing)
    printf("Data written by the writer %d is %d\n", f, x);
    syscall(335,f,”Data written by the writer\n”);


    
    // Exit section
    sem_post(&wblock); // Release wblock to allow other writers
}
 
int main() {
    int i;
    pthread_t id1[5], id2[5]; // id1 = reader , id2 = writer threads
    
    //Initializing
    sem_init(&mutex, 0, 1);   
    sem_init(&wblock, 0, 1);  
    
    //Create reader and writer threads
    for (i = 0; i <= 2; i++) {
        pthread_create(&id2[i], NULL, writer, &i);
        pthread_create(&id1[i], NULL, reader, &i);
    }
    
    //Join reader and writer threads
    for (i = 0; i <= 2; i++) {
        pthread_join(id2[i], NULL);
        pthread_join(id1[i], NULL);
    }
    
    //Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wblock);
 
    return 0;
}
