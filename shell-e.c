#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

typedef struct Child
{
  int pid;
  int status;
  int option;
} Child;

struct Child child[1024];
pid_t pid, wpid, cpid;
// int *b;
int status, child_length;

void handler(int sig)
{
  if (cpid != 0)
  {
    printf("receive SIGINT\n");
    kill(cpid, SIGINT);
  }
}

void handler2(int sig)
{
  while ((wpid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    if (WIFEXITED(status) != 0)
    {
      printf("終了[%d]\n", wpid);

      for (int i = 0; i < child_length; i++)
      {
        if (child[i].pid == wpid)
        {
          child[i].status = 1;
          break;
        }
      }
    }
  }
  printf("receive SIGCHLD\n");
}

int main()
{
  int count, fg_id, child_length;

  char cmd[1024], *tp;

  char quit[10] = "quit";
  char ex[10] = "exit";
  char job[10] = "jobs";

  char *pargs[1024];

  for (int i = 0; i < 1024; i++)
  {
    child[i].pid = 0;
    child[i].status = 0;
    child[i].option = 0;
  }

  while (1)
  {
    cpid = 0;
    signal(SIGINT, handler);
    signal(SIGCHLD, handler2);

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
      for (int i = 0; i < child_length; i++)
      {
        char child_bf[100];
        char child_s[100];

        if (child[i].option == 1)
        {
          strcpy(child_bf, "foreground");
        }
        else
        {
          strcpy(child_bf, "background");
        }

        if (child[i].status == 1)
        {
          strcpy(child_s, "DONE");
        }
        else
        {
          strcpy(child_s, "RUNNING");
          printf("PID:%d Status:%s Option:%s\n", child[i].pid, child_s, child_bf);
        }
      }
    }
    else if (strcmp("fg", pargs[0]) == 0)
    {
      fg_id = atoi(pargs[1]);
      cpid = fg_id;

      while ((wpid = waitpid(fg_id, &status, 0)) > 0)
      {
        if (WIFEXITED(status) != 0)
        {
          printf("終了[%d]\n", wpid);
          for (int i = 0; i < child_length; i++)
          {
            if (child[i].pid == wpid)
            {
              child[i].status = 1;
              break;
            }
          }
        }
      }
    }
    else
    {
      pid = fork();
      if (strcmp(pargs[count - 1], "&") == 0)
      //バックグラウンド実行
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
          child[child_length].pid = pid;
          child[child_length].status = 0;
          child[child_length].option = 0;
          child_length++;
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
          cpid = pid;
          waitpid(cpid, &status, 0);
          child[child_length].pid = pid;
          child[child_length].status = 1;
          child[child_length].option = 1;
          child_length++;
        }
      }
    }
  }
}