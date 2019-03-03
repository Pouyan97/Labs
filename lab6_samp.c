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

