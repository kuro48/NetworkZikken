#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
  pid_t pid;
  int status;

  char cmd[1024];

  char quit[10] = "quit";
  char ex[10] = "exit";

  char *pargs[2] = {cmd, NULL};

  while (1)
  {
    printf(">");
    scanf("%s", cmd);
    if (strcmp(quit, cmd) == 0 || strcmp(ex, cmd) == 0)
    {
      return 0;
    }

    pid = fork();

    if (pid < 0)
    {
      perror("error");
    }
    else if (pid == 0)
    {
      execv(cmd, pargs);
      perror(cmd);
      exit(EXIT_FAILURE);
    }
    else
    {
      wait(&status);
    }
  }
}