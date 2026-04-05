#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Test program that deliberately wastes memory to demonstrate
// eco memory monitoring. Allocates memory using lazy sbrk
// but only touches some of it, creating unmapped (wasted) pages.

int
main(int argc, char *argv[])
{
  int alloc_kb = 64;  // default: allocate 64KB lazily

  if(argc > 1)
    alloc_kb = atoi(argv[1]);

  if(alloc_kb <= 0 || alloc_kb > 1024){
    printf("memwaste: usage: memwaste [KB to waste, 1-1024]\n");
    exit(1);
  }

  printf("memwaste: pid %d allocating %d KB lazily...\n", getpid(), alloc_kb);

  // Allocate memory lazily (SBRK_LAZY = 2)
  char *mem = sbrklazy(alloc_kb * 1024);
  if(mem == (char*)-1){
    printf("memwaste: sbrk failed\n");
    exit(1);
  }

  // Only touch the first 4KB (1 page) so the rest remains unmapped
  // This simulates a process that reserves lots of memory but barely uses it
  printf("memwaste: touching only first page of %d KB allocation\n", alloc_kb);
  for(int i = 0; i < 4096; i++){
    mem[i] = (char)(i & 0xff);
  }

  printf("memwaste: done. Run 'ecomem' to see the waste report.\n");
  printf("memwaste: sleeping for 100 ticks so you can check...\n");

  pause(100);

  printf("memwaste: exiting\n");
  exit(0);
}
