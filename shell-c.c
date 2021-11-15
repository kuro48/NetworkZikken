#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct child
{
  int child_id;
  int child_status;
  
};

int main()
{
  pid_t pid, wpid;
  int status, count, fg_id;

  char cmd[1024], *tp;

  char quit[10] = "quit";
  char ex[10] = "exit";
  char job[10] = "jobs";

  char *pargs[1024];

  while (1)
  {
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
      if (WIFEXITED(status) != 0)
      {
        printf("終了[%d]\n", pid);
      }
    }
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
    else if (strcmp(job, pargs[0]) == 0)
    {
      for (i = 0; i < p_n; i++)
      {
        //プロセスIDの表示
        printf("%d\n", pid_id[i]);
      }
    }

    else if (strcmp("fg", pargs[0]) == 0)
    {
      printf("FG実行したいプロセスの番号：");
      fgets(fg, sizeof(fg), stdin);
      fg_id = atoi(fg);

      waitpid(fg_id, &status, WNOHANG);
    }
    else
    {
      pid = fork();

      if (strcmp(pargs[count - 1], "&") == 0)
      {
        pargs[count - 1] = NULL;
        if (pid < 0)
        {
          perror("error");
        }
        else if (pid == 0)
        {
          execv(pargs[0], pargs);
          perror(pargs[0]);
          exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
          printf("%d", pid);
        }
      }
      else
      {
        if (pid < 0)
        {
          perror("error");
        }
        else if (pid == 0)
        {
          execv(pargs[0], pargs);
          perror(pargs[0]);
          exit(EXIT_SUCCESS);
        }
        else
        {
          wait(&status);
          printf("%d", pid);
        }
      }
    }
  }
}