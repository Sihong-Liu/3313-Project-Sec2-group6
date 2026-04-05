#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sensor.h"
#include "user/user.h"

#define MONITOR_DURATION_TICKS 150
#define TICKS_PER_SECOND 10

static void
format_time_from_ticks(int ticks, int *hours, int *minutes, int *seconds)
{// Convert ticks to hours, minutes, and seconds for display when reading is taken
  int total_seconds;

  total_seconds = ticks / TICKS_PER_SECOND;
  *hours = total_seconds / 3600;
  *minutes = (total_seconds % 3600) / 60;
  *seconds = total_seconds % 60;
}

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
  int hours;
  int minutes;
  int seconds;
  int start_ticks;

  start_ticks = uptime();
  for(;;){
    if(read_stats(SENSOR_TYPE_TEMPERATURE, &temp) < 0)
      exit(1);
    if(read_stats(SENSOR_TYPE_AIR_QUALITY, &air) < 0)
      exit(1);
    if(read_stats(SENSOR_TYPE_ENERGY_USAGE, &energy) < 0)
      exit(1);

    printf("[Sensor Monitor]\n");
    format_time_from_ticks(temp.latest_tick, &hours, &minutes, &seconds);
    printf("Temp latest: %d avg: %d count: %d time: %d:%d:%d\n",
           temp.latest, temp.avg, temp.count, hours, minutes, seconds);
    format_time_from_ticks(air.latest_tick, &hours, &minutes, &seconds);
    printf("Air latest: %d avg: %d count: %d time: %d:%d:%d\n",
           air.latest, air.avg, air.count, hours, minutes, seconds);
    format_time_from_ticks(energy.latest_tick, &hours, &minutes, &seconds);
    printf("Energy latest: %d avg: %d count: %d time: %d:%d:%d\n",
           energy.latest, energy.avg, energy.count, hours, minutes, seconds);

    if(temp.count > 0 && temp.latest > SENSOR_TEMP_WARNING)
      printf("WARNING: Temperature too high\n");
    if(air.count > 0 && air.latest > SENSOR_AIR_WARNING)
      printf("WARNING: Air quality too high\n");
    if(energy.count > 0 && energy.latest > SENSOR_ENERGY_WARNING)
      printf("WARNING: Energy usage too high\n");

    printf("\n");
    if(uptime() - start_ticks >= MONITOR_DURATION_TICKS){
      printf("sensor_monitor: stopping after %d ticks\n", MONITOR_DURATION_TICKS);
      exit(0);
    }
    pause(20);
  }
}
