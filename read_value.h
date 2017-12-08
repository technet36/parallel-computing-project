
#ifndef PARALLEL_COMPUTING_PROJECT_READ_VALUE_H
#define PARALLEL_COMPUTING_PROJECT_READ_VALUE_H



#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "errno.h"
#include "structures.h"
//#define TRACE
#define FILE_PREFIX "worlds/world"
#define FILE_SUFFIX ".bin"
#define CONFIG_FILENAME "config.ini"

Config_t * testConfig(Config_t*);
Config_t* initConfig(Config_t*);
int read_value(char*);
void clean_worlds_dir();
int export_world_t(World_t my_world, int, Config_t*);

#endif //PARALLEL_COMPUTING_PROJECT_READ_VALUE_H
