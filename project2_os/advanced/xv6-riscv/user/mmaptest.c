// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"

// int
// main(int argc, char *argv[])
// {
//   int pid;
//   uint64 shared_addr;

//   printf("Testing mmap shared memory...\n");

//   // Map shared memory
//   shared_addr = mmap();
//   if(shared_addr == 0){
//     printf("mmap failed\n");
//     exit(1);
//   }

//   printf("Mapped shared memory at %p\n", (void*)shared_addr);

//   // Write to shared memory
//   int *shared_data = (int*)shared_addr;
//   *shared_data = 42;
//   printf("Parent wrote: %d\n", *shared_data);

//   // Fork a child process
//   pid = fork();
//   if(pid < 0){
//     printf("fork failed\n");
//     exit(1);
//   }

//   if(pid == 0){
//     // Child process
//     printf("Child read: %d\n", *shared_data);

//     // Modify the shared data
//     *shared_data = 100;
//     printf("Child wrote: %d\n", *shared_data);

//     // Test unmapping in child
//     if(munmap(shared_addr) < 0)
//       printf("Child: munmap failed\n");
//     else
//       printf("Child: munmap succeeded\n");

//     exit(0);
//   } else {
//     // Parent process
//     wait(0);

//     // Verify the child's modification is visible
//     printf("Parent read after child: %d\n", *shared_data);

//     // Unmap shared memory
//     if(munmap(shared_addr) < 0)
//       printf("Parent: munmap failed\n");
//     else
//       printf("Parent: munmap succeeded\n");
//   }

//   exit(0);
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_REGIONS 3

int
main(int argc, char *argv[])
{
  int pid;
  uint64 addrs[NUM_REGIONS];

  printf("Testing MULTIPLE dynamic mmap shared memory regions...\n");

  // 1. Map multiple shared memory regions
  for(int i = 0; i < NUM_REGIONS; i++) {
    addrs[i] = mmap();
    if(addrs[i] == 0){
      printf("mmap %d failed\n", i);
      exit(1);
    }
    printf("Mapped shared region %d at %p\n", i, (void*)addrs[i]);
  }

  // 2. Verify the kernel gave us distinct dynamic addresses
  if(addrs[0] == addrs[1] || addrs[1] == addrs[2] || addrs[0] == addrs[2]) {
    printf("Error: mmap returned overlapping or identical addresses!\n");
    exit(1);
  }

  // 3. Write distinct initial values to each shared memory region
  for(int i = 0; i < NUM_REGIONS; i++) {
    int *shared_data = (int*)addrs[i];
    *shared_data = (i + 1) * 10; // Writes 10, 20, 30
    printf("Parent wrote to region %d: %d\n", i, *shared_data);
  }

  // 4. Fork a child process
  pid = fork();
  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // CHILD PROCESS
    for(int i = 0; i < NUM_REGIONS; i++) {
      int *shared_data = (int*)addrs[i];
      
      // Verify parent's write
      printf("Child read from region %d: %d\n", i, *shared_data);
      
      // Modify the shared data
      *shared_data = (i + 1) * 100; // Writes 100, 200, 300
      printf("Child wrote to region %d: %d\n", i, *shared_data);
    }

    // Test unmapping in child
    for(int i = 0; i < NUM_REGIONS; i++) {
      if(munmap(addrs[i]) < 0)
        printf("Child: munmap region %d failed\n", i);
      else
        printf("Child: munmap region %d succeeded\n", i);
    }

    exit(0);
  } else {
    // PARENT PROCESS
    wait(0);

    // Verify the child's modifications are visible
    for(int i = 0; i < NUM_REGIONS; i++) {
      int *shared_data = (int*)addrs[i];
      printf("Parent read from region %d after child: %d\n", i, *shared_data);
    }

    // Unmap shared memory in parent
    for(int i = 0; i < NUM_REGIONS; i++) {
      if(munmap(addrs[i]) < 0)
        printf("Parent: munmap region %d failed\n", i);
      else
        printf("Parent: munmap region %d succeeded\n", i);
    }
  }

  printf("Advanced mmap test completed successfully!\n");
  exit(0);
}