#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sensor.h"
#include "user/user.h"

static int
next_air_value(int step)
{
  int span = 180 - 20 + 1;
  int seed = uptime() + getpid() * 23 + step * 29;

  return 20 + (seed % span);
}

int
main(void)
{
  int step;

  step = 0;
  for(;;){
    if(submitsensor(SENSOR_TYPE_AIR_QUALITY, next_air_value(step)) < 0){
      fprintf(2, "air_sensor: submit failed\n");
      exit(1);
    }
    step++;
    pause(24);
  }
}
