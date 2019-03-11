#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for(int i=0;i<2;i++) // loop will run n times (n=5)
    {
        pid_t childPIDOrZero = fork();
        if(childPIDOrZero != 0) {
            //Parent Process
	    //wait(NULL);
            printf("I am the parent %d, my child is %d \n",getpid(), childPIDOrZero);
	    //printf("I am the parent %d, my child is %d \n",getpid(), getppid());
	    //printf("I am the parent %d, my child is %d \n",getppid(), getpid());
            wait(NULL);
        }else{
            //Child Process
            printf("I am the child %d, my parent is %d \n",getpid(), getppid());
        }
    }
    return 0;
}