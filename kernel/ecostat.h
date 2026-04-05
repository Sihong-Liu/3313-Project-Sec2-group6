#ifndef _ECOSTAT_H_
#define _ECOSTAT_H_

#include "types.h"

// Per-process eco statistics exported to user space.
struct ecostat {
  int pid;
  char name[16];
  uint64 total_cpu_ticks;
  int busy_score;
  int cooldown_ticks;
  int times_throttled;
};

#endif
