#ifndef RANDOM_HAO_H
#define RANDOM_HAO_H

#define SIMPLE_SPRNG

#ifdef MPI_HAO
#include <mpi.h> 
#define USE_MPI
#endif

#include "sprng.h"

void random_hao_init(int seed=985456376, int gtype=1);
double uniform_hao();
double gaussian_hao();

#endif
