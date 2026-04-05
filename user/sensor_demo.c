#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//this is a startup program for environmental monitoring subsystem, 
//it spawns the three sensor processes and the monitor process that reads from them and displays the data
static void
spawn(char *prog)
{
  int pid;
  char *argv[2];

  argv[0] = prog;
  argv[1] = 0;

  pid = fork();
  if(pid < 0){
    fprintf(2, "sensor_demo: fork failed for %s\n", prog);
    exit(1);
  }

  if(pid == 0){
    exec(prog, argv);
    fprintf(2, "sensor_demo: exec failed for %s\n", prog);
    exit(1);
  }
}

int
main(void)
{
  char *monitor_argv[2];

  printf("sensor_demo: starting temp_sensor, air_sensor, energy_sensor, and sensor_monitor\n");

  spawn("temp_sensor");
  spawn("air_sensor");
  spawn("energy_sensor");
  pause(30);

  monitor_argv[0] = "sensor_monitor";
  monitor_argv[1] = 0;
  exec("sensor_monitor", monitor_argv);

  fprintf(2, "sensor_demo: exec failed for sensor_monitor\n");
  exit(1);
}
