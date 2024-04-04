Spring 2024 CIS 345/545 Project 1

In this project, you are asked to write two independent programs, thr_atomic.c and thr_reduce.c, each of which uses m computational threads to concurrently calculate the sum af quadruple roots from 1 to n, where m 
and n are powers of 2 and are specified in the command line. For the program thr_atomic.c, it gets the values m and n from the command line arguments and converts them
to two integers, respectively. Next, it creates m threads using pthread_create() and each thread computes the sum of n/m quadruple roots. Namely, the first thread (i.e. thread 0) computes the sum of quadruple roots from 1 to
n/m, the second thread (i.e. thread 1) computes the sum of the quadruple roots from n/m+1 to 2n/m, etc. When a thread finishes its computation, it should print its partial sum and atomically add it to a shared global variable.
Finally, your program uses pthread_join() to let the main thread know that all of the m computational threads have done the atomic additions and then it can print the final result. Below is an example of running your thread program:

bachs> ./thr_atomic 2 65536

thr 0: 352703.926537

thr 1: 486164.553017

sum of quadruple roots: 838868.479554

The program thr_reduce.c is similar to thr-atomic.c except that you need to use the parallel reduction approach to 
combine the partial sums. That is, your program uses a shared global array and each computational
thread stores its partial sum, but not prints, in an array element indexed on its thread ID. Then, half of these threads
call pthread_join() to wait for their corresponding partner threads completion, and then each of these threads
can add two numbers in the array together. This reduction procedure will be performed log_2(m) times and each time
the number of the active threads will be reduced half. Finally, there will be only one active thread and this thread
should print the whole array.

For example, assume that there are 8 computational threads. In the first reduction step, in order to add two
elements in the array, thread 4 should wait for thread 0 done, thread 5 has to wait for thread 1 done, thread 6
needs to wait for thread 2 done, and thread 7 should wait for thread 3 done. In the second reduction step, thread 6
waits for thread 4 to finish, and thread 7 waits for thread 5 to finish. In the third step, thread 7 waits for thread 6
done and then prints the whole array. Hint: to find its partner thread ID during the ith reduction step, a thread
can use its ID XORed with 2^(t-i), where r=log(m). Furthermore, you have to use bit-shift operations instead of
calling log() or pow() related math library functions in your program.
Note that the main thread just calls pthread_exit() after creating m threads. It does not need to wait for these
threads to finish. Calling pthread_exit() in the main thread will allow other threads to continue execution.
