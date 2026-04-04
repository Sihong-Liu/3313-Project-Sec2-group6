#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sensor.h"
#include "user/user.h"

static int
next_energy_value(int step)
{
  int span = 95 - 10 + 1;
  int seed = uptime() + getpid() * 19 + step * 37;

  return 10 + (seed % span);
}

int
main(void)
{
  int step;

  step = 0;
  for(;;){
    if(submitsensor(SENSOR_TYPE_ENERGY_USAGE, next_energy_value(step)) < 0){
      fprintf(2, "energy_sensor: submit failed\n");
      exit(1);
    }
    step++;
    pause(28);
  }
}

