#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//this is a startup program for environmental monitoring subsystem,
//it spawns the three sensor processes and the monitor process that reads from them and displays the data
static int
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

  return pid;
}

int
main(void)
{
  int temp_pid;
  int air_pid;
  int energy_pid;
  int monitor_pid;
  int waited_pid;

  printf("sensor_demo: starting temp_sensor, air_sensor, energy_sensor, and sensor_monitor\n");

  temp_pid = spawn("temp_sensor");
  air_pid = spawn("air_sensor");
  energy_pid = spawn("energy_sensor");
  pause(30);//wait for sensors to run and collect data
  monitor_pid = spawn("sensor_monitor");

  for(;;){// Wait for the monitor to finish before killing the sensors
    waited_pid = wait((int *) 0);
    if(waited_pid < 0)
      break;
    if(waited_pid == monitor_pid)
      break;
  }
//kill sensor processes after ten ticks finished.
  kill(temp_pid);
  kill(air_pid);
  kill(energy_pid);
  while(wait((int *) 0) >= 0)
    ;
  exit(0);
}
