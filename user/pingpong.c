
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])

{
  int p[2];
  pipe(p);
  int pid = fork();
  char buf[1] = {'a'};


  if(pid == 0){ //child
     pid = getpid();
     read(p[0],buf,1);
     close(p[0]);
     fprintf(2, "%d: received ping\n", pid);
     write(p[1], buf, 1);
     close(p[1]);
     exit(0);

  }else{ // parent
    pid = getpid();
    write(p[1], buf, 1);
    close(p[1]);

    read(p[0],buf,1);
    close(p[0]);
    fprintf(2, "%d: received pong\n", pid);

  }

}
