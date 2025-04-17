#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// get the base name from a path
char* basename(char *path){
  char *p = path + strlen(path);
  while (p >= path && *p != '/'){
    p--;
  }
  return p+1;
}

char* clean_name(char *fileName){
  static char p[DIRSIZ + 1];
  memmove(p, fileName, DIRSIZ);
  p[DIRSIZ] = 0;
  for (int i = DIRSIZ-1; i >= 0; i--){
    if(p[i] == ' '){
      p[i] = 0;
    }
    else{
    break;
    }
  }
  return p;
}




void
find(char *path, char *fileName){
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;

if(strcmp(basename(path), fileName) == 0){
    printf("%s\n", path);
}

if((fd = open(path, 0)) < 0){
    close(fd);
    return;
}
if(fstat(fd, &st) < 0){
    close(fd);
    return;
}

if(st.type != T_DIR){
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
            char *name = clean_name(de.name);
            int len = strlen(name);
            memmove(p, name, len);
            p[len] = 0;
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
  find(argv[1], argv[2]);
  exit(0);
}



