#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  volatile uint64 x = 0;

  for(int round = 0; round < 50; round++){
    for(uint64 i = 0; i < 2000000ULL; i++){
      x += i;
    }
    pause(1);
  }

  if(x == 0)
    printf("politework: %lu\n", x);
  exit(0);
}
