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

      for (int d = 0; d < height; d++) {

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
