
for (int k = 0; k < height; k++) {

          sum = sum + A[c][k]*B[k][d];

        }


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

    float *A,*B,*C; //matrices A,B and C

int i = 0;
        while(i< 32)
        {
            A[i] = rand() % 100;
            B[i] =A[i];
            i++; 

        }