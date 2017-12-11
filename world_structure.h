//use not to have multiple declaration of the file
#ifndef PARALLEL_COMPUTING_PROJECT_WORLD_STRUCTURE_H
#define PARALLEL_COMPUTING_PROJECT_WORLD_STRUCTURE_H

//#define TRACE

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "structures.h"


pthread_barrier_t childBarrier;

World_t create_world_t (Config_t* );
void* all_steps(void *);
void display_world_t(World_t ,Config_t* );
void delete_world_t();

#endif //PARALLEL_COMPUTING_PROJECT_WORLD_STRUCTURE_H
