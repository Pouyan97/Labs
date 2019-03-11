#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, const char *argv[])
{


    //Opening the file and checking for errors
    //We are supposed to pass permission to open for the file
    int fd = open("fooBar.txt", O_CREAT|O_WRONLY, 0666);
    if (fd == -1){
        printf("file not found.\n");
        return -1;
    }

    //Replacing the descriptor table entry for stdout with fd
    dup2(fd, 1);

    //Forking and checking for errors
    pid_t pid = fork();
    if(pid<0){
        exit(1);
    }

    if(pid == 0){
        //Child process
        char *args[2];
        args[0] = "/bin/ls";        // first arg is the full path to the executable
        args[1] = NULL;             // list of args must be NULL terminated
        execv( args[0], args );

        //Close the descriptor in the child process
        close(fd);
    }else{
        //Parent Process
        wait(NULL);
        //This still gets printed to the file as the descriptor for parent process is still open
        printf("May be this");

        close(fd);

    }

    return 0;
}
