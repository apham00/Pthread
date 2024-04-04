#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

void *summation(void *arg);

int m, n; //m = # of threads, n = number we are calculating with
double *sum; //declaring pointer sum to resize it into an array
pthread_t *thr; //declaring pointer thread to also resize it into an array

int main(int argc, char *argv[]){
   //simple conversion from string to int
   m = atoi(argv[1]);
   n = atoi(argv[2]);
   //malloc conversion allows us to resize array to allow access of the global
   //array to the summation class
   sum = (double *)malloc(m*sizeof(double));
   thr = (pthread_t *)malloc(m*sizeof(pthread_t));
   //doing minus now to make it so bigger calculations get done quicker
   //and bigger # of threads work correctly
   for (int i = (m - 1); i >= 0; i--){
      //creating # of threads user specified and passing which thread (i) we are in
      pthread_create(&thr[i], NULL, summation, (void *)(long)i);
   }
   /*since the other threads will complete before we have a finished result
   we will need to stop until all the threads are done with summation class
   pthread_exit will allow the program to wait until every thread is done*/
   pthread_exit(NULL);
   return 0;
}

void *summation(void *arg){
   long tid = (long) arg; //get # id to know which thread we are in to print
   int start = (n/m) * tid + 1; //calculating where is the start and stop in the thread with the id
   int stop = (n/m) * (tid + 1);
   for (int i = start; i <= stop; i++){
      //calculating the quad root of each number from start to finish in m different sums
      sum[tid] += pow(i, 0.25);
   }
   //this way makes it so sum[0] is the main sum
   for (int d = (m / 2); d >= 1; d /= 2) {
      /*we want the top half of threads to be
      done with the summation class
      so this if is stating that if the number
      is not the lower half of threads then leave*/
      if (tid >= d){
         break;
      }
      //xor finds the partner. so 0 is partnered with 4 if d = 4
      int partner = tid ^ d;
      //we do not want to put summation in tid until we know partner is done
      pthread_join(thr[partner], NULL);
      sum[tid] += sum[partner];
      /*with the way we have our program, we are putting all the additions
      tid 0. When the threads are done with all the calculations, we know that
      d = 1 and there cannot be anything else to calculate*/
      if (d == 1){
         for (int i = 0; i < m; i ++){
            printf("thr %d = %f\n", i, sum[i]);
         }
         printf("sum: %f\n", sum[0]);
      }
   }
}
