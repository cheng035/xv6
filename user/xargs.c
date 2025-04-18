
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_SIZE 1024
int main(int argc, char *argv[])

{

  if (argc>MAXARG){
    fprintf(3,"wrong two many agrs");
  }

  char *child_argv[20]; // store the previous argv.
  char *p;
  char buf[MAX_SIZE];
  int pid = fork();

  // c
  int pointer; // record the inital pointer here
  for (pointer = 0; pointer < argc - 1; pointer++)
  {
    child_argv[pointer] = argv[pointer + 1];
  }

  int anchor = pointer;
  int previous_start = 0;
  int n = read(0, buf, sizeof(buf));
  for (int i = 0; i < MAX_SIZE; i++)
  {
    if (buf[i] != ' ' && buf[i] != '\n')
    {
      continue;
    }

    else if (buf[i] == ' ')
    {
      buf[i] = "\0";
      child_argv[pointer++] = &buf[previous_start];
      previous_start = i + 1;
    }

    else
    { // buf[i] == '\n'
      child_argv[pointer] = 0;
      int pid = fork();
      if (pid == 0)
      { // child

        exec(argv[1], child_argv);
        exit(0);
      }
      else
      {
        wait(0);
        // to do reset child_argv
        pointer = anchor;
      }
    }
  }
  exit(0);
}
