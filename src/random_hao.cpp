#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <string>
#include "random_hao.h"

using namespace std;

void random_hao_init(int seed, int gtype)
{
#ifdef MPI_HAO
    MPI_Bcast(&seed,1,MPI_INT,0,MPI_COMM_WORLD );
    MPI_Bcast(&gtype,1,MPI_INT,0,MPI_COMM_WORLD );
#endif

    if(seed==-1) //If seed is -1, start from last run
    {  
        random_hao_read(); 
    }
    else if(seed==0) //If seed is 0, random set the seed
    {
        seed = make_sprng_seed();
        init_sprng(gtype,seed,SPRNG_DEFAULT);
    }
    else
    {
        init_sprng(gtype,seed,SPRNG_DEFAULT);
    }
}

void random_hao_read()
{
    int size;
    char buffer[MAX_PACKED_LENGTH];

    int rank=0; 
#ifdef MPI_HAO
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
    string filename="random_checkpoint_"+ to_string(rank) +".dat";
    FILE *fp = fopen(filename.c_str(),"r"); //use the example code from sprng
    fread(&size,1,sizeof(int),fp);
    fread(buffer,1,size,fp);
    unpack_sprng(buffer);
    fclose(fp);
}

void random_hao_save()
{
    int size;
    char *bytes;

    size = pack_sprng(&bytes);

    int rank=0;
#ifdef MPI_HAO
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
    string filename="random_checkpoint_"+ to_string(rank) +".dat";
    FILE *fp = fopen(filename.c_str(),"w"); //use the example code from sprng 
    fwrite(&size,1,sizeof(int),fp); 
    fwrite(bytes,1,size,fp); 
    fclose(fp);
    free(bytes);	
}

void random_hao_backup()
{
    int rank=0;
#ifdef MPI_HAO
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
#endif

    //Use MPI_Barrier before and after, make sure all seeds has been generated and has not been removed.
    if(rank == 0) system("mkdir -p random.bk"); 
    if(rank == 0) system("mv random_checkpoint* random.bk"); 

#ifdef MPI_HAO
    MPI_Barrier(MPI_COMM_WORLD);
#endif
}

double uniform_hao()
{
    return sprng();
}

double gaussian_hao()
{
#define pi 3.14159265358979324
    double tmp=sprng();
    //log(tmp) will be NA, if tmp==0
    if(tmp==0) tmp+=1e-300;
    return sqrt(-2.0*log(tmp))*cos(2.0*pi*sprng());
}
