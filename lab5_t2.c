#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    char *cmd1[] = { "/bin/ls", NULL};
    char *cmd2[] = { "/usr/bin/wc", NULL};

    int fd[2];

    pipe(fd);

    int pid1 = fork();
    switch (pid1) {
        case 0: /* child */
            dup2(fd[1], 1);    /* this end of the pipe becomes the standard output */
            close(fd[0]);     /* this process don't need the other end */
            execvp(cmd1[0], cmd1); /* run the command */
            exit(0);
        default: /* parent does nothing */
            break;
        case -1:
            //error
            exit(1);
    }

    int pid2 = fork();
    switch (pid2) {
        case 0: /* child */
            dup2(fd[0], 0);    /* this end of the pipe becomes the standard input */
            close(fd[1]);     /* this process doesn't need the other end */
            execvp(cmd2[0], cmd2); /* run the command */
            exit(0);
        default: /* parent does nothing */
            break;
        case -1:
            //error
            exit(1);
    }

    close(fd[0]); close(fd[1]);    /* this is important! close both file descriptors on the pipe */

    exit(0);
}
