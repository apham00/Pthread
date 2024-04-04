#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
void *summation(void *arg);

int m, n; //m = # of threads, n = number we are calculating with
double sum;
//lock is used to make sure there is no race condition
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
   //simple conversion from string to int
   m = atoi(argv[1]);
   n = atoi(argv[2]);
   pthread_t thr[m]; //initializing an array of threads to how many user specified
   for (int i = 0; i < m; i ++){
      //creating # of threads user specified and passing which thread (i) we are in
      pthread_create(&thr[i], NULL, summation, (void *)(long)i);
   }
   for (int i = 0; i < m; i++){
      //stops the main class and waits for all threads to finish
      pthread_join(thr[i], NULL);
   }
   printf("sum = %f\n", sum);
   return 0;
}

void *summation(void *arg){
   long tid = (long) arg; //get # id to know which thread we are in to print
   int start = (n/m) * tid + 1; //calculating where is the start and stop in the thread with the id
   int stop = (n/m) * (tid + 1);
   double local_sum; //used to make it so we lock and unlock the program once per thread
   for (int i = start; i <= stop; i++){
      //calculating the quad root of each number from start to finish
      local_sum += pow(i, 0.25);
   }
   printf("thr %ld = %f\n", tid, local_sum);
   pthread_mutex_lock(&lock); //locking adding the local_sum to sum because of race condition
   sum += local_sum;
   pthread_mutex_unlock(&lock); //unlocking means other threads can now use global sum
}