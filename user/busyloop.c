#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  volatile uint64 x = 0;

  printf("busyloop: pid=%d\n", getpid());

  for(int round = 0; round < 20; round++){
    for(uint64 i = 0; i < 200000000ULL; i++){
      x += i;
    }
  }

  if(x == 0)
    printf("busyloop: %lu\n", x);
  exit(0);
}
