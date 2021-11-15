#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define MAX 1024

int main()
{
  pid_t pid, wpid;
  int status;
  char *str;
  char quit[10] = "quit";
  char ex[10] = "exit";

  char *get_c;

  char cmd[1024];

  while (1)
  {
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
      if (WIFEXITED(status) != 0)
      {
        printf("終了[%d]\n", pid);
      }
    }

    char *pargs[MAX];

    for (int i = 0; i < MAX; i++)
    {
      pargs[i] = NULL;
    }

    printf(">");
    fgets(cmd, MAX, stdin);
    str = strtok(cmd, " ");

    int i = 0;
    for (i = 0; str != NULL; i++)
    {
      pargs[i] = str;
      str = strtok(NULL, " ");
    }

    pargs[i - 1] = strtok(pargs[i - 1], "\n");

    if (strcmp(quit, pargs[0]) == 0 || strcmp(ex, pargs[0]) == 0)
    {
      exit(EXIT_SUCCESS);
    }
    else
    {

      pid = fork();

      if (strcmp(pargs[i - 1], "&") == 0)
      {
        pargs[i - 1] = NULL;

        if (pid == 0)
        {
          sleep(10);
          execv(pargs[0], pargs);
          exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
          // Error forking
          perror("lsh");
        }
      }
      else
      {
        if (pid == 0)
        {
          execv(pargs[0], pargs);
          exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
          // Error forking
          perror("lsh");
        }
        else
        {
          wait(&status);
        }
      }
    }
  }
}