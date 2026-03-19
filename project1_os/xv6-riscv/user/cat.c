// #include "kernel/types.h"
// #include "kernel/fcntl.h"
// #include "user/user.h"

// char buf[512];

// void
// cat(int fd)
// {
//   int n;

//   while((n = read(fd, buf, sizeof(buf))) > 0) {
//     if (write(1, buf, n) != n) {
//       fprintf(2, "cat: write error\n");
//       exit(1);
//     }
//   }
//   if(n < 0){
//     fprintf(2, "cat: read error\n");
//     exit(1);
//   }
// }

// int
// main(int argc, char *argv[])
// {
//   int fd, i;

//   if(argc <= 1){
//     cat(0);
//     exit(0);
//   }

//   for(i = 1; i < argc; i++){
//     if((fd = open(argv[i], O_RDONLY)) < 0){
//       fprintf(2, "cat: cannot open %s\n", argv[i]);
//       exit(1);
//     }
//     cat(fd);
//     close(fd);
//   }
//   exit(0);
// }


#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAXLINE 512

int
readline(int fd, char *buf, int maxlen)
{
  int n;
  char c;
  int i = 0;

  while((n = read(fd, &c, 1)) > 0){
    buf[i++] = c;
    if(c == '\n')
      break;
    if(i >= maxlen - 1){
      fprintf(2, "line too long\n");
      exit(1);
    }
  }

  if((n == 0 && i == 0) || n < 0)
    return n;

  buf[i] = '\0';
  return i;
}

void
print_lineno(int n)
{
  int digits = 0;
  int t = n;

  if(t == 0) digits = 1;
  while(t > 0){
    digits++;
    t /= 10;
  }

  // print leading spaces
  for(int i = 0; i < 6 - digits; i++)
    printf(" ");

  printf("%d  ", n);
}

void
cat(int fd, int linenum)
{
  char line[MAXLINE];
  int n;
  static int cnt = 1;

  while((n = readline(fd, line, MAXLINE)) > 0){
    if(linenum)
      print_lineno(cnt++);
    printf("%s", line);
  }
}

int
main(int argc, char *argv[])
{
  int fd;
  int i = 1;
  int linenum = 0;

  if(argc > 1 && strcmp(argv[1], "-n") == 0){
    linenum = 1;
    i = 2;
  }

  if(i >= argc){
    cat(0, linenum);   // stdin
    exit(0);
  }

  for(; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    }
    cat(fd, linenum);
    close(fd);
  }

  exit(0);
}
