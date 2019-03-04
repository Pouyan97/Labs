#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<termios.h>
#include<string.h>


#define TRUE 1
#define FALSE 0
/*

Lab #6 - Foreground and Background processes.
Today we're going to scratch the surface of job control in unix - like
systems.

In the good old days all we had was the shell, but users were still able to
multitask (run multiple programs at once). Today we're going to show you how
that was done.

Key to the idea of job control is process groups. A process group is a
collection of one or more processes
(https://en.wikipedia.org/wiki/Process_group). What makes this interesting
is you can set process groups to be active on the terminal! Today we'll
show you how to do it.

So far, we've worked in a single process group. You can assign a process to
its own process group using the command

setpgid(pid_t pid, pid_t gpid)

where pid is the process id of the process you want to add to the group and
gpid is the group process id (highest process in the heierarchy of the group).
When pid == gpid you start a new process group with gpid the same as pid.

Lets fork off a process and set it to its own process group.

pid = fork();
if(pid == 0)
{
  pid = getpid() // pid is initialized to zero in the Fork
  setpgid(pid, pid);
  // Do stuff
  exit(0);
}
if(pid < 0)
{
  perror("fork");
  exit(1);
}
setpgid(pid, pid); // WOAH! What's going on here???
// Do stuff

Race Condition
We've touched on this in the past - a race condition is when concurrent
processes need to synchronize but their execution order is non-deterministic.
In this scenario we don't know if the parent or the child will set the process
group first - but both have enough information to set it correctly. This is why
we set the process group in both.

Foreground Processes
Now that we've created a new process group we can make it active in the
terminal. For that we use the system system call
tcsetpgrp(int fd, pid_t pgrp)
Where fd is the file descriptor of the terminal (STDOUT_FILENO in our case)
and pgrp is the process group id of the process group you want to give
control.
Lets add it into our fork from above.

pid = fork();
if(pid == 0)
{
  pid = getpid() // pid is initialized to zero in the Fork
  setpgid(pid, pid);
  tcsetpgrp(STDIN_FILENO, pid); // And now you're under control
  // And then you do what they told you.
  exit(0);
}
if(pid < 0)
{
  perror("fork");
  exit(1);
}
setpgid(pid, pid); // WOAH! What's going on here???
// Do stuff

NICE! You've given the process group control of the terminal and its running
in the foreground.

The lab 6 assignment is in three sections:
1. Finish the code so that control is returned to the main process
  before exiting.
3. Why must we block SIGTTIN and SIGTTOU to be able to transfer control?
  - Try removing the signal handler registers and see what it does.
2. Modify the code to take an input parameter to send a job to the background.
  Send a job to the background.

Code attribution - http://www.gnu.org/software/libc/manual/html_node/Launching-Jobs.html#Launching-Jobs
*/

int main(int argc, char* argv[])
{
  // Usage - type in the arguments for the program you want to test
  pid_t parent_pgid, child_pid, pid;
  int status, background;

  // Block SIGTTIN and SIGTTOUT - Why do these interfere?
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);

  // set the parent to its own process group
  parent_pgid = getpid();
  setpgid(parent_pgid, parent_pgid);

  // Give this parent thread control of the terminal
  tcsetpgrp(STDIN_FILENO, parent_pgid);

  if(argc == 1)
  {
    fprintf(stderr, "Usage - ./lab6 command_to_run and arguments\n");
    return 0;
  }

  // Test that we've properly redirected the terminal
  printf("Hello from the parent process\n");

  // Fork the child
  child_pid = fork();
  if(child_pid == 0)
  {
      printf("Hello from the fork\n");
      child_pid = getpid();
      setpgid(child_pid, child_pid);
      tcsetpgrp(STDIN_FILENO, child_pid);
      printf("Hello from child process\n");
      execvp(argv[1], &(argv[1]));
      fprintf(stderr, "Unable to execute %s\n", argv[1]);
  }
  if(child_pid < 0)
  {
    perror("Fork error");
    exit(1);
  }
  // Set the child to its own process group.
  setpgid(child_pid, child_pid);
  if(!background)
    waitpid(-1, &status, 0);

  // Get control of the terminal back
  printf("Hello from the other side\n");

  return(0);
}

