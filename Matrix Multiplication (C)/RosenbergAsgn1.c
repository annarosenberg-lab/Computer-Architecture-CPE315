#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>




int matMult (int **C, int **A, int **B, int height, int width)
{
    int sum;

    for (int c = 0; c < height; c++) {

      for (int d = 0; d < width; d++) {

        for (int k = 0; k < height; k=k+2) {

          sum = sum + A[c][k]*B[k][d];
          sum = sum + A[c][k+1]*B[k+1][d];


        }

        *(*(C + c) + d) = sum;

        sum = 0;

      }

    }
    return 0;
}


int main(){
    int **A, **B, **C;
    int height = 256;
    int width = 256;
    int i;

    //Initialize matrixs A and B
    A = malloc(sizeof(int*) * (height));
    for (i = 0; i < height; i++)
    A[i] = malloc(sizeof(int) * (height));
  
    B = malloc(sizeof(int*) * (height));
    for (i = 0; i < height; i++)
    B[i] = malloc(sizeof(int) * (height));
  
    C = malloc(sizeof(int*) * (height));
    for (i = 0; i < height; i++)
    C[i] = malloc(sizeof(int) * (height));


    
    for(int o=0;o<height;o++)
    {
   for(int p=0;p<height;p++)
   {
       *(*(B + o) + p) = rand() % 2;
       *(*(A + o) + p) = rand() % 2;
   }
    }
    

    //Multiply matrixs
    matMult (C, A, B, height, width);




    return 0;
}

