#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // 找到最后一个 '/' 后的名字
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


void
find(char *path, char *fileName){
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;
    if(strcmp(fmtname(path), fileName) == 0){
         printf("%s\n", path);
    }
    if((fd = open(path, 0)) < 0){
       return;
     }

     if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
      }

     if(st.type == T_DIR) {
          if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
          }
          strcpy(buf, path);
          p = buf+strlen(buf);
          *p++ = '/';

          while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
              continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf,fileName);
          }
          close(fd);

     }
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("wrong parameter");
    exit(1);
  }
  find(argv[1], fmtname(argv[2]));
  exit(0);
}



