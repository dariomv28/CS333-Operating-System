#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/getproc.h"

char*
statestr(enum procstate s)
{
  switch(s){
    case UNUSED: return "UNUSED";
    case USED: return "USED";
    case SLEEPING: return "SLEEP";
    case RUNNABLE: return "RUNNABLE";
    case RUNNING: return "RUN";
    case ZOMBIE: return "ZOMBIE";
  }
  return "UNKNOWN";
}

int
main(void)
{
  struct procinfo p[64];
  int n;

  n = getprocs(p, 64);
  if(n < 0){
    printf("ps: getprocs failed\n");
    exit(1);
  }

  printf("PID\tSTATE\tSIZE\tNAME\n");
  for(int i = 0; i < n; i++){
    printf("%d\t%s\t%lu\t%s\n", p[i].pid, statestr(p[i].state), p[i].sz, p[i].name);
  }

  exit(0);
}
