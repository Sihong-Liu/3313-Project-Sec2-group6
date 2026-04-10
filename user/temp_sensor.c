#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sensor.h"
#include "user/user.h"

static int next_temperature_value(int step){
  int span = 35 - 18 + 1;
  int seed = uptime() + getpid() * 17 + step * 31;

  return 18 + (seed % span);
}

int main(void){
  int step;

  step = 0;
  for(;;){
    if(submitsensor(SENSOR_TYPE_TEMPERATURE, next_temperature_value(step)) < 0){
      fprintf(2, "temp_sensor: submit failed\n");
      exit(1);
    }
    step++;
    pause(20);
  }
}
