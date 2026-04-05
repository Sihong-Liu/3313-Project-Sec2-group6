#ifndef XV6_SENSOR_H
#define XV6_SENSOR_H

// Sihong's Code Begin: Environmental monitoring shared definitions
#define SENSOR_TYPE_TEMPERATURE 1
#define SENSOR_TYPE_AIR_QUALITY 2
#define SENSOR_TYPE_ENERGY_USAGE 3

#define SENSOR_TYPE_MIN SENSOR_TYPE_TEMPERATURE
#define SENSOR_TYPE_MAX SENSOR_TYPE_ENERGY_USAGE

#define SENSOR_BUFFER_SIZE 64

#define SENSOR_TEMP_WARNING 30
#define SENSOR_AIR_WARNING 120
#define SENSOR_ENERGY_WARNING 80

struct sensor_data {
  int type;
  int value;
  int tick;
};

struct sensor_stats {
  int latest;
  int count;
  int avg;
  int latest_tick;
};
// Sihong's Code End: Environmental monitoring shared definitions

#endif
