#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// attributes: noreturn tells the compiler this function calls exit() 
// and doesn't return, which silences the infinite recursion warning.
__attribute__((noreturn))
void
sieve(int pleft)
{
  int p; // số prime hiện tại
  int n;
  
  // Read the first number from the left pipe.
  // This number must be prime.
  if(read(pleft, &p, sizeof(p)) == 0){
    // If read returns 0, the pipeline has ended.
    exit(0);
  }

  printf("prime %d\n", p);

  // tạo pipe phải, với 0 là đọc, 1 là ghi
  int pright[2];
  pipe(pright);

  if(fork() == 0){
    // --- Child Process (Right Neighbor) ---
    
    // Close the write-end of the new pipe (child reads from it)
    close(pright[1]);
    
    // Close the read-end of the old pipe (child doesn't need it)
    close(pleft);
    
    // Recursively run the sieve for the next stage
    sieve(pright[0]);
  } else {
    // --- Parent Process (Current Filter) ---
    
    // Close the read-end of the new pipe (parent writes to it)
    close(pright[0]);

    // Read numbers from the left pipe
    while(read(pleft, &n, sizeof(n)) != 0){
      // If n is not divisible by our prime p, pass it to the right
      if(n % p != 0){
        write(pright[1], &n, sizeof(n));
      }
    }

    // Close the inputs and outputs to signal EOF to the child
    close(pleft);
    close(pright[1]);

    // Wait for the child to exit to ensure the pipeline finishes entirely
    wait(0);
    
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  if(fork() == 0){
    // --- First Child (Filter for 2) ---
    close(p[1]); // Close write end
    sieve(p[0]);
  } else {
    // --- Main Generator Process ---
    close(p[0]); // Close read end
    
    // Feed numbers 2 through 280 into the pipeline
    for(int i = 2; i <= 280; i++){
      write(p[1], &i, sizeof(i));
    }
    
    // Close write end to signal end of data
    close(p[1]);
    
    // Wait for the first child to finish
    wait(0);
  }
  
  exit(0);
}