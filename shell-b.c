#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
  pid_t pid;
  int status, count;

  if (argc == !1)
  {
    exit(EXIT_FAILURE);
  }

  char cmd[1024], *tp;

  char quit[10] = "quit";
  char ex[10] = "exit";

  char *pargs[1024];

  while (1)
  {
    //配列の初期化
    for (int i = 0; i < 1024; i++)
    {
      pargs[i] = NULL;
    }

    printf(">");
    fgets(cmd, 1024, stdin);
    tp = strtok(cmd, " ");

    for (count = 0; tp != NULL; count++)
    {
      pargs[count] = tp;
      tp = strtok(NULL, " ");
    }

    pargs[count - 1] = strtok(pargs[count - 1], "\n");

    if (strcmp(quit, pargs[0]) == 0 || strcmp(ex, pargs[0]) == 0)
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
      execv(pargs[0], pargs);
      perror(pargs[0]);
      exit(EXIT_FAILURE);
    }
    else
    {
      wait(&status);
    }
  }
}