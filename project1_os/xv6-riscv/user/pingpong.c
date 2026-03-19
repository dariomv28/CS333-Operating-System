#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int p2c[2]; // parent -> child
  int c2p[2]; // child -> parent
  char buf;

  // create pipes
  pipe(p2c);
  pipe(c2p);

  int pid = fork();

  if (pid == 0) {
    // CHILD
    read(p2c[0], &buf, 1);
    printf("%d: received ping\n", getpid());
    write(c2p[1], &buf, 1);
    exit(0);
  } else {
    // PARENT
    buf = 'x';
    write(p2c[1], &buf, 1);
    read(c2p[0], &buf, 1);
    printf("%d: received pong\n", getpid());
    exit(0);
  }
}
