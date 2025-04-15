
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void getPrime(int rfd){

    int prime;
    int num;
    int p2[2];
    int n;


    //get the first prime number
    n = read(rfd, &prime, 4);
    if (n==0){ // end the recursion, no more number
            close(rfd);
            exit(0);
    }

        pipe(p2);

    fprintf(2, "prime %d\n", prime);

    //get the rest prime numbers
    if ( n != 0){
        int pid = fork(); // create a child
        if (pid == 0){ // this is a child process
            close(p2[1]);
            getPrime(p2[0]);
            close(p2[1]);
            exit(0);
    }
    else{ // parent, put the number in
         for(;;){
            n = read(rfd, &num, 4);
            if (n==0){
                close(rfd);
                close(p2[1]);
                break;
            }
            if (num % prime != 0){
                write(p2[1], &num, 4);
            }
        }
    }
    wait(0); // 等子进程跑完
    close(p2[0]);
    exit(0);
    }
}



int
main(int argc, char *argv[])

{
    int p[2];
    pipe(p);
    for (int i = 2; i <= 35; i++){
        write(p[1], &i, 4);
    }
    close(p[1]);
    getPrime(p[0]);
    close(p[0]);
    wait(0); // 等子进程跑完
    exit(0);
}
