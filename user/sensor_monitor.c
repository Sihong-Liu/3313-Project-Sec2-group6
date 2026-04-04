#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sensor.h"
#include "user/user.h"

static int
read_stats(int type, struct sensor_stats *stats)
{
  if(getsensorstats(type, stats) < 0){
    fprintf(2, "sensor_monitor: failed to read sensor type %d\n", type);
    return -1;
  }
  return 0;
}

int
main(void)
{
  struct sensor_stats temp;
  struct sensor_stats air;
  struct sensor_stats energy;

  for(;;){
    if(read_stats(SENSOR_TYPE_TEMPERATURE, &temp) < 0)
      exit(1);
    if(read_stats(SENSOR_TYPE_AIR_QUALITY, &air) < 0)
      exit(1);
    if(read_stats(SENSOR_TYPE_ENERGY_USAGE, &energy) < 0)
      exit(1);

    printf("[Sensor Monitor]\n");
    printf("Temp latest: %d avg: %d count: %d\n", temp.latest, temp.avg, temp.count);
    printf("Air latest: %d avg: %d count: %d\n", air.latest, air.avg, air.count);
    printf("Energy latest: %d avg: %d count: %d\n", energy.latest, energy.avg, energy.count);

    if(temp.count > 0 && temp.latest > SENSOR_TEMP_WARNING)
      printf("WARNING: Temperature too high\n");
    if(air.count > 0 && air.latest > SENSOR_AIR_WARNING)
      printf("WARNING: Air quality too high\n");
    if(energy.count > 0 && energy.latest > SENSOR_ENERGY_WARNING)
      printf("WARNING: Energy usage too high\n");

    printf("\n");
    pause(50);
  }
}

