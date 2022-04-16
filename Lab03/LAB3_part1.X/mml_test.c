// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"




// User libraries:
#include "MatrixMath.h"


// Module-level variables:
float zero_matrix[3][3] = {{},{},{}};
int MatrixEquals2(float mat1[DIM-1][DIM-1], float mat2[DIM-1][DIM-1]);
                       
    

int main()
{
    
    BOARD_Init();
    
    float mat1[DIM][DIM] = {{1,2,3},{1,2,3},{1,2,3}};
    float mat2[DIM][DIM] = {{2,3,4},{2,3,4},{2,3,4}};
    float mat3[DIM][DIM] = {{1,2,3},{0,1,4},{5,6,0}};
    float mat4[DIM][DIM] = {{-24,18,5},{20,-15,-4},{-5,4,1}};
    float resultadd1[DIM][DIM];
    float resultadd2[DIM][DIM];
    float resultscalaradd1[DIM][DIM];
    float resultscalaradd2[DIM][DIM];
    float resultmatrixmultiply1[DIM][DIM];
    float resultmatrixmultiply2[DIM][DIM];
    float resultscalarmultiply1[DIM][DIM];
    float resultscalarmultiply2[DIM][DIM];
    float resulttranspose1[DIM][DIM];
    float resulttranspose2[DIM][DIM];
    float resultsub1[DIM-1][DIM-1];
    float resultsub2[DIM-1][DIM-1];
    float resultinverse1[DIM][DIM];
    float resultinverse2[DIM][DIM];
    float x = 1;
    
    
    //MatrixAdd  
    float expected_add1[DIM][DIM] = {{3,5,7},{3,5,7},{3,5,7}};
    float expected_add2[DIM][DIM] = {{3,5,7},{3,5,7},{3,5,7}};
    MatrixAdd(mat1, mat2, resultadd1);
    MatrixAdd(mat2, mat1, resultadd2);
    
    //MatrixScalarAdd
    float expected_scalaradd1[DIM][DIM] = {{2,3,4},{2,3,4},{2,3,4}};
    float expected_scalaradd2[DIM][DIM] = {{3,4,5},{3,4,5},{3,4,5}};
    MatrixScalarAdd(x, mat1, resultscalaradd1);
    MatrixScalarAdd(x, mat2, resultscalaradd2);
    
    //MatrixMultiply
    float expected_matrixmult1[DIM][DIM] = {{12,18,24},{12,18,24},{12,18,24}};
    float expected_matrixmult2[DIM][DIM] = {{9,18,27},{9,18,27},{9,18,27}};
    MatrixMultiply(mat1, mat2, resultmatrixmultiply1);
    MatrixMultiply(mat2, mat1, resultmatrixmultiply2);
    
    //MatrixScalarMultiply
    float expectedscalarmult1[DIM][DIM] = {{1,2,3},{1,2,3},{1,2,3}};
    float expectedscalarmult2[DIM][DIM] = {{2,3,4},{2,3,4},{2,3,4}};
    MatrixScalarMultiply(x, mat1, resultscalarmultiply1);
    MatrixScalarMultiply(x, mat2, resultscalarmultiply2);
    
    //MatrixTrace
    float expectedtrace1 = 6;
    float expectedtrace2 = 9;
    float trace1 = MatrixTrace(mat1);
    float trace2 = MatrixTrace(mat2);
    
    //MatrixTranspose
    float expectedtranspose1[DIM][DIM] = {{1,1,1},{2,2,2},{3,3,3}};
    float expectedtranspose2[DIM][DIM] = {{2,2,2},{3,3,3},{4,4,4}};
    MatrixTranspose(mat1, resulttranspose1);
    MatrixTranspose(mat2, resulttranspose2);
    
    //MatrixSubmatrix
    float expectedsub1[DIM-1][DIM-1] = {{1,3},{5,0}};
    float expectedsub2[DIM-1][DIM-1] = {{-15,-4},{4,1}};
    MatrixSubmatrix(1, 1, mat3, resultsub1);
    MatrixSubmatrix(0, 0, mat4, resultsub2);
    
    //MatrixDeterminant
    float expecteddeterminant1 = 1;
    float expecteddeterminant2 = 0;
    float determinant1 = MatrixDeterminant(mat3);
    float determinant2 = MatrixDeterminant(mat2);
    
    //MatrixInverse
    float expectedinverse1[DIM][DIM] = {{-24,18,5},{20,-15,-4},{-5,4,1}};
    float expectedinverse2[DIM][DIM] = {{1,2,3},{0,1,4},{5,6,0}};
    MatrixInverse(mat3, resultinverse1);
    MatrixInverse(mat4, resultinverse2);
            
   
    printf("Beginning CRUZID's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    printf("\nDemonstrating MatrixPrint():\n");

    // Add more tests here!
    int equals_test = 0;
    int add_test = 0;
    int scalar_addtest = 0;
    int matrix_multiplytest = 0;
    int scalar_multiplytest = 0;
    int passed_functions = 0;
    int trace_test = 0;
    int transpose_test = 0;
    int submatrix_test = 0;
    int determinant_test = 0;
    int inverse_test = 0;
    
    //MatrixEquals
    if (MatrixEquals(mat1, mat1)== TRUE){
        equals_test++;
    }
    if (MatrixEquals(mat2, mat2)== TRUE){
        equals_test++;
    }
    
    if (MatrixEquals(mat1, mat1)== TRUE && MatrixEquals(mat2, mat2)== TRUE){
        passed_functions++;
    }
    
    //MatrixAdd
    if (MatrixEquals(resultadd1, expected_add1)== TRUE){
        add_test++;
    }
    if (MatrixEquals(resultadd2, expected_add2)== TRUE){
        add_test++;
    }
    if (MatrixEquals(resultadd1, expected_add1)== TRUE && MatrixEquals(resultadd2, expected_add2)== TRUE){
        passed_functions++;
    }
    
    //MatrixScalarAdd
    if (MatrixEquals(resultscalaradd1, expected_scalaradd1)== TRUE){
        scalar_addtest++;
    }
    if (MatrixEquals(resultscalaradd2, expected_scalaradd2)== TRUE){
        scalar_addtest++;
    }
    if (MatrixEquals(resultscalaradd1, expected_scalaradd1)== TRUE && MatrixEquals(resultscalaradd2, expected_scalaradd2)== TRUE){
        passed_functions++;
    }
    
    //MatrixMultiply
    if (MatrixEquals(resultmatrixmultiply1, expected_matrixmult1)== TRUE){
        matrix_multiplytest++;
    }
    if (MatrixEquals(resultmatrixmultiply2, expected_matrixmult2)== TRUE){
        matrix_multiplytest++;
    }
    if (MatrixEquals(resultmatrixmultiply1, expected_matrixmult1)== TRUE && MatrixEquals(resultmatrixmultiply2, expected_matrixmult2)== TRUE){
        passed_functions++;
    }   
    
    //MatrixScalarMultiply
    if (MatrixEquals(resultscalarmultiply1, expectedscalarmult1)== TRUE){
        scalar_multiplytest++;
    }
    if (MatrixEquals(resultscalarmultiply2, expectedscalarmult2)== TRUE){
        scalar_multiplytest++;
    }
    if (MatrixEquals(resultscalarmultiply1, expectedscalarmult1)== TRUE && MatrixEquals(resultscalarmultiply2, expectedscalarmult2)== TRUE){
        passed_functions++;
    }  
    
    //MatrixTrace
    if (trace1 == expectedtrace1){
        trace_test++;
    }
    if (trace2 == expectedtrace2){
        trace_test++;
    }
    if (trace1 == expectedtrace1 && trace2 == expectedtrace2){
        passed_functions++;
    }
    
    //MatrixTranspose
    if (MatrixEquals(resulttranspose1, expectedtranspose1)== TRUE){
        transpose_test++;
    }
    if (MatrixEquals(resulttranspose2, expectedtranspose2)== TRUE){
        transpose_test++;
    }
    if (MatrixEquals(resulttranspose1, expectedtranspose1)== TRUE && MatrixEquals(resulttranspose2, expectedtranspose2)== TRUE){
        passed_functions++;
    }
    
    //MatrixSubmatrix
    if (MatrixEquals2(resultsub1, expectedsub1)== TRUE){
        submatrix_test++;
    }
    if (MatrixEquals2(resultsub2, expectedsub2)== TRUE){
        submatrix_test++;
    }
    if (submatrix_test == 2){
        passed_functions++;
    }
    
    //MatrixDeterminant
    if ((determinant1 - expecteddeterminant1)<= FP_DELTA) {
        determinant_test++;
    }
    if ((determinant2 - expecteddeterminant2)<= FP_DELTA){
        determinant_test++;
    }
    if (determinant_test == 2){
        passed_functions++;
    }
    
    //MatrixInverse
    if (MatrixEquals(resultinverse1, expectedinverse1)== TRUE){
        inverse_test++;
    }
    if (MatrixEquals(resultinverse2, expectedinverse2)== TRUE){
        inverse_test++;
    }
    if (inverse_test == 2){
        passed_functions++;
    }
    
    //print statements
    printf("\nPASSED (%d/2): MatrixEquals():\n", equals_test);
    printf("PASSED (%d/2): MatrixAdd():\n", add_test);
    printf("PASSED (%d/2): MatrixScalarAdd():\n", scalar_addtest);
    printf("PASSED (%d/2): MatrixMultiply():\n", matrix_multiplytest);
    printf("PASSED (%d/2): MatrixScalarMultiply():\n", scalar_multiplytest);
    printf("PASSED (%d/2): MatrixTrace():\n", trace_test);
    printf("PASSED (%d/2): MatrixTranspose():\n", transpose_test);
    printf("PASSED (%d/2): MatrixSubmatrix():\n", submatrix_test);
    printf("PASSED (%d/2): MatrixDeterminant():\n", determinant_test);
    printf("PASSED (%d/2): MatrixInverse():\n", inverse_test);
    printf("\n-----------------------------------\n");
    float percent = passed_functions;
    printf("%d out of 10 functions passed (%0.2f%%)\n", passed_functions, (percent/10.0)*100);
    printf("\nOutput of MatrixPrint():\n");
    MatrixPrint(mat1);
    printf("\nExpected output of MatrixPrint():\n");
    printf(" --------------------\n");
    printf("| 1.00 | 2.00 | 3.00 |\n");
    printf(" --------------------\n");
    printf("| 1.00 | 2.00 | 3.00 |\n");
    printf(" --------------------\n");
    printf("| 1.00 | 2.00 | 3.00 |\n");
    printf(" --------------------");
    
    BOARD_End();
    while (1);
}

