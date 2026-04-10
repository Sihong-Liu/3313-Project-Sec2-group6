#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/ecostat.h"
#include "user/user.h"

static void
print_pad(int n)
{
  for(int i = 0; i < n; i++)
    printf(" ");
}

static int
intlen(int x)
{
  int len = 1;
  if(x < 0){
    len++;
    x = -x;
  }
  while(x >= 10){
    x /= 10;
    len++;
  }
  return len;
}

static int
u64len(uint64 x)
{
  int len = 1;
  while(x >= 10){
    x /= 10;
    len++;
  }
  return len;
}

static void
print_int_pad(int x, int width)
{
  int len = intlen(x);
  printf("%d", x);
  if(len < width)
    print_pad(width - len);
}

static void
print_u64_pad(uint64 x, int width)
{
  int len = u64len(x);
  printf("%lu", x);
  if(len < width)
    print_pad(width - len);
}

static void
print_str_pad(const char *s, int width)
{
  int len = strlen(s);
  printf("%s", s);
  if(len < width)
    print_pad(width - len);
}

int
main(void)
{
  struct ecostat stats[NPROC];
  int n = ecopstat(stats, NPROC);

  if(n < 0){
    fprintf(2, "ecops: ecopstat failed\n");
    exit(1);
  }

  print_str_pad("PID", 6);
  print_str_pad("NAME", 16);
  print_str_pad("CPU", 12);
  print_str_pad("BUSY", 6);
  print_str_pad("COOLDOWN", 10);
  print_str_pad("THROTTLED", 10);
  // Energy-Aware Scheduler: show how many quanta each process has consumed.
  // A high ENERGY value means the process may be deferred under low load.
  print_str_pad("ENERGY", 8);
  printf("\n");
  for(int i = 0; i < n; i++){
    print_int_pad(stats[i].pid, 6);
    print_str_pad(stats[i].name, 16);
    print_u64_pad(stats[i].total_cpu_ticks, 12);
    print_int_pad(stats[i].busy_score, 6);
    print_int_pad(stats[i].cooldown_ticks, 10);
    print_int_pad(stats[i].times_throttled, 10);
    print_int_pad(stats[i].energy_score, 8);
    printf("\n");
  }

  exit(0);
}
