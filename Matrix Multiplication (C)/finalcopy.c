#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MATRIX_DIMENSION_XY 10
//SEARCH FOR TODO
//
//***********************************************************************************
//*************************************
// sets one element of the matrix
void set_matrix_elem(float *M,int x,int y,float f)
{
M[x + y*MATRIX_DIMENSION_XY] = f;
}
//
//***********************************************************************************
//*************************************
// lets see it both are the same
int quadratic_matrix_compare(float *A,float *B)
{
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
       if(A[a +b * MATRIX_DIMENSION_XY]!=B[a +b * MATRIX_DIMENSION_XY]) 
        return 0;
   
return 1;
}
//
//***********************************************************************************
//*************************************
//print a matrix
void quadratic_matrix_print(float *C)
{
    printf("\n");
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    {
    printf("\n");
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
        printf("%.2f,",C[a + b* MATRIX_DIMENSION_XY]);
    }
printf("\n");
}
//
//***********************************************************************************
//*************************************
// multiply two matrices
void quadratic_matrix_multiplication(float *A,float *B,float *C)
{
//nullify the result matrix first
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
        C[a + b*MATRIX_DIMENSION_XY] = 0.0;
//multiply
for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++) // over all rows b
        for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
            {
                C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a 
+ c*MATRIX_DIMENSION_XY]; 
            }
}
//
//***********************************************************************************
//*************************************
void synch(int par_id,int par_count,int *ready)
{
//TODO: synch algorithm. make sure, ALL processes get stuck here until all ARE here
    int id = ++ready[par_id];
    int i = 0;
    while(i<par_count)
    {
        while(ready[i] < id);
        i++;
    }

}

void quadratic_matrix_multiplication_parallel(int par_id, int par_count,float* A,float * B,float* C, int* ready)
{

     int split = MATRIX_DIMENSION_XY/par_count;
    int rem = MATRIX_DIMENSION_XY % par_count;
    if (par_id == 0)
        {
            for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
            for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
            C[a + b*MATRIX_DIMENSION_XY] = 0.0;
        }
    
    if(rem)
    {
        if(par_id == 0)
        {
            split += rem;
            for(int a = 0; a<MATRIX_DIMENSION_XY; a++) 
                for(int b = 0; b < MATRIX_DIMENSION_XY/par_count + rem; b++) 
                    for(int c = 0;c<MATRIX_DIMENSION_XY;c++) 
                        {
                            C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a 
                            + c*MATRIX_DIMENSION_XY]; 
                        }
        }
        else
        {
            for(int a = 0; a<MATRIX_DIMENSION_XY; a++) 
                for(int b = (par_id * split) + rem; b<(((par_id + 1) * split)  + rem); b++) 
                    for(int c = 0;c<MATRIX_DIMENSION_XY;c++) 
                        {
                            C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a 
                            + c*MATRIX_DIMENSION_XY]; 
                        }
        }

    }
    else
    {
        
        
        
        for(int a = 0; a<MATRIX_DIMENSION_XY; a++) 
            for(int b = (par_id * split); b<(par_id * split) + split; b++) 
                for(int c = 0;c<MATRIX_DIMENSION_XY;c++) 
                    {
                        C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a 
                        + c*MATRIX_DIMENSION_XY]; 
                    }
        
    }



}
//
//***********************************************************************************
//*************************************
int main(int argc, char *argv[])
{
int par_id = 0; // the parallel ID of this process
int par_count = 1; // the amount of processes
float *A,*B,*C; //matrices A,B and C
int *ready; //needed for synch




if(argc!=3){printf("no shared\n");}
else
    {
    par_id= atoi(argv[1]);
    par_count= atoi(argv[2]);
    }
if(par_count==1){printf("only one process\n");}
int fd[4];
if(par_id==0)
    {
    //TODO: init the shared memory for A,B,C, ready. shm_open with C_CREAT here! then ftruncate! then mmap
    fd[0] = shm_open("matrixA", O_RDWR | O_CREAT, 0777);
    ftruncate(fd[0], MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4);
    A = (float*)mmap(0, MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd[0], 0);
    fd[1] = shm_open("matrixB", O_RDWR | O_CREAT, 0777);
    ftruncate(fd[1], MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4);
    B = (float*)mmap(0, MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd[1], 0);
    fd[2] = shm_open("matrixC", O_RDWR | O_CREAT, 0777);
    ftruncate(fd[2], MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4);
    C = (float*)mmap(0, MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd[2], 0);
    
    fd[3] = shm_open("Ready", O_RDWR | O_CREAT, 0777);
    ftruncate(fd[3], 4 * par_count);
    ready = (int*)mmap(0, 4 * par_count, PROT_READ | PROT_WRITE, MAP_SHARED, fd[3], 0);
    
    }
else
    {
//TODO: init the shared memory for A,B,C, ready. shm_open withOUT C_CREAT here! NO ftruncate! but yes to mmap
    sleep(2); //needed for initalizing synch
    fd[0] = shm_open("matrixA", O_RDWR | O_CREAT, 0777);
    A = (float*)mmap(0, MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd[0], 0);
    fd[1] = shm_open("matrixB", O_RDWR | O_CREAT, 0777);
    B = (float*)mmap(0, MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd[1], 0);
    fd[2] = shm_open("matrixC", O_RDWR | O_CREAT, 0777);
    C = (float*)mmap(0, MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd[2], 0);
    fd[3] = shm_open("Ready", O_RDWR, 0777);
    ready = (int*)mmap(0, 4 * par_count, PROT_READ | PROT_WRITE, MAP_SHARED, fd[3], 0);
    
    }
synch(par_id,par_count,ready);
if(par_id==0)
    {
//TODO: initialize the matrices A and B
        int i = 0;
        int bound = MATRIX_DIMENSION_XY*MATRIX_DIMENSION_XY;
        while(i< bound)
        {
            A[i] = 1;
            B[i] = 1;
            i++;
        }
    }
synch(par_id,par_count,ready);
//TODO: quadratic_matrix_multiplication_parallel(par_id, par_count,A,B,C, ...);
quadratic_matrix_multiplication_parallel(par_id, par_count,A,B,C,ready);

synch(par_id,par_count,ready);
if(par_id==0)
    quadratic_matrix_print(C);
synch(par_id, par_count, ready);
//lets test the result:fd[3] = shm_open("Ready", O_RDWR, 0777);
float M[MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY];
quadratic_matrix_multiplication(A, B, M);
if (quadratic_matrix_compare(C, M))
printf("full points!\n");
else
printf("buuug!\n");
close(fd[0]);
close(fd[1]);
close(fd[2]);
close(fd[3]);
shm_unlink("matrixA");
shm_unlink("matrixB");
shm_unlink("matrixC");
shm_unlink("synchobject");
return 0;    
}