
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])

{
  int p[2];
  pipe(p);
  int pid = fork();
  char buf[1];
  char b;
  if(pid > 0){ //child
     b = read(p[0],buf,1);
     close(p[1]);
     fprintf(3, "%d: received ping\n", pid);
     write(1, b, p[1])
     close(p[0]);
     exit(0);
  }else{
    pid = getpid();
    write(1, 'a', p[1])
    close(p[0]);
    b = read(p[0],buf,1);
    fprintf(3, "%d: received pong\n", pid);
    close(p[1]);
  }

}
