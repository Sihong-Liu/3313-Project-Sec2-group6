#include "types.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "sensor.h"

// Sihong's Code Begin: Environmental monitoring subsystem implementation
struct sensor_totals {
  int latest;
  int sum;
  int count;
};

static struct spinlock sensorlock;
static struct sensor_data sensor_buffer[SENSOR_BUFFER_SIZE];
static struct sensor_totals sensor_totals_by_type[SENSOR_TYPE_MAX + 1];
static int sensor_write_index;
static int sensor_count;

static int
sensor_type_valid(int type)
{
  return type >= SENSOR_TYPE_MIN && type <= SENSOR_TYPE_MAX;
}

void
sensorinit(void)
{
  initlock(&sensorlock, "sensor");
  sensor_write_index = 0;
  sensor_count = 0;
}

int
sensor_submit(int type, int value)
{
  int tick_snapshot;
  struct sensor_totals *stats;

  if(!sensor_type_valid(type))
    return -1;

  acquire(&tickslock);
  tick_snapshot = ticks;
  release(&tickslock);

  acquire(&sensorlock);

  sensor_buffer[sensor_write_index].type = type;
  sensor_buffer[sensor_write_index].value = value;
  sensor_buffer[sensor_write_index].tick = tick_snapshot;
  sensor_write_index = (sensor_write_index + 1) % SENSOR_BUFFER_SIZE;
  if(sensor_count < SENSOR_BUFFER_SIZE)
    sensor_count++;

  stats = &sensor_totals_by_type[type];
  stats->latest = value;
  stats->sum += value;
  stats->count += 1;

  release(&sensorlock);
  return 0;
}
// Sihong's Code End: Environmental monitoring subsystem implementation

int
sensor_get_stats(int type, struct sensor_stats *out)
{
  struct sensor_totals *stats;

  if(!sensor_type_valid(type) || out == 0)
    return -1;

  acquire(&sensorlock);

  stats = &sensor_totals_by_type[type];
  out->latest = stats->latest;
  out->count = stats->count;
  if(stats->count > 0)
    out->avg = stats->sum / stats->count;
  else
    out->avg = 0;

  release(&sensorlock);
  return 0;
}
