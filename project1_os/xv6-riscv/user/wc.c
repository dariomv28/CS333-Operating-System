// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "kernel/fcntl.h"
// #include "user/user.h"

// char buf[512];

// void
// wc(int fd, char *name)
// {
//   int i, n;
//   int l, w, c, inword;

//   l = w = c = 0;
//   inword = 0;
//   while((n = read(fd, buf, sizeof(buf))) > 0){
//     for(i=0; i<n; i++){
//       c++;
//       if(buf[i] == '\n')
//         l++;
//       if(strchr(" \r\t\n\v", buf[i]))
//         inword = 0;
//       else if(!inword){
//         w++;
//         inword = 1;
//       }
//     }
//   }
//   if(n < 0){
//     printf("wc: read error\n");
//     exit(1);
//   }
//   printf("%d %d %d %s\n", l, w, c, name);
// }

// int
// main(int argc, char *argv[])
// {
//   int fd, i;

//   if(argc <= 1){
//     wc(0, "");
//     exit(0);
//   }

//   for(i = 1; i < argc; i++){
//     if((fd = open(argv[i], O_RDONLY)) < 0){
//       printf("wc: cannot open %s\n", argv[i]);
//       exit(1);
//     }
//     wc(fd, argv[i]);
//     close(fd);
//   }
//   exit(0);
// }


#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

int opt_l = 0;
int opt_w = 0;
int opt_c = 0;

void
wc(int fd, char *name)
{
  int i, n;
  int l = 0, w = 0, c = 0;
  int inword = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i = 0; i < n; i++){
      c++;
      if(buf[i] == '\n')
        l++;
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;
        inword = 1;
      }
    }
  }

  if(n < 0){
    printf("wc: read error\n");
    exit(1);
  }

  if(opt_l) printf("%d ", l);
  if(opt_w) printf("%d ", w);
  if(opt_c) printf("%d ", c);

  if(name && name[0] != 0)
    printf("%s", name);

  printf("\n");
}

int
main(int argc, char *argv[])
{
  int fd;
  int i;
  int has_file = 0;

  // parse options
  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      for(int j = 1; argv[i][j] != 0; j++){
        if(argv[i][j] == 'l') opt_l = 1;
        else if(argv[i][j] == 'w') opt_w = 1;
        else if(argv[i][j] == 'c') opt_c = 1;
        else{
          printf("wc: invalid option -- %c\n", argv[i][j]);
          exit(1);
        }
      }
    }
  }

  // default
  if(!opt_l && !opt_w && !opt_c){
    opt_l = opt_w = opt_c = 1;
  }

  // process files
  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-')
      continue;

    has_file = 1;
    fd = open(argv[i], O_RDONLY);
    if(fd < 0){
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i]);
    close(fd);
  }

  // stdin
  if(!has_file){
    wc(0, "");
  }

  exit(0);
}

// line, word, characters