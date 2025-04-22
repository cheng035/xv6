
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_SIZE 1024
 
// echo hello1 | xagrs cho hello2
int main(int argc, char *argv[])

{
  if (argc > 50)
  {
    fprintf(2, "wrong too many agrs");
    exit(0);
  }

  if (argc < 2){
    fprintf(2, "you need at least two parameters");
    exit(0);
  }

  char *child_argv[20]; // store the previous argv.
  char buf[MAX_SIZE];
  // c
  int pointer; // record the inital pointer here
  for (pointer = 0; pointer < argc - 1; pointer++)
  {
    child_argv[pointer] = argv[pointer + 1];
  }

  int anchor = pointer;
  int previous_start = 0;
  int n;
  while ((n = read(0, buf, sizeof(buf))) > 0)
  {

    for (int i = 0; i < n; i++)
    {
      char c = buf[i];
      if (c != ' ' && c != '\n')
      {
        continue;
      }

      else 
      {
        buf[i] = '\0';
        child_argv[pointer++] = &buf[previous_start];
        previous_start = i + 1;
      }

      if (c == '\n')
      { 
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
  }
  exit(0);
}
