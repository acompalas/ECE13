/* 
 * File:   MatrixMath.c
 * Author: ajcom
 *
 * Created on April 13, 2022, 9:54 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "MatrixMath.h"
#include <math.h>
#include <string.h>
#define TRUE 1
#define FALSE 0


/*
 * 
 */

void getCofactor(float mat[DIM][DIM], float result[DIM][DIM]);
int MatrixEquals2(float mat1[DIM-1][DIM-1], float mat2[DIM-1][DIM-1]);

void MatrixAdd(float mat1[DIM][DIM], float mat2[DIM][DIM], float result[DIM][DIM]){
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           result[i][j] = mat1[i][j]+ mat2[i][j];
       }
    }
        
}

void MatrixScalarAdd(float x, float mat[DIM][DIM], float result[DIM][DIM]){
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           result[i][j] = mat[i][j]+ x;
       }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[DIM][DIM], float result[DIM][DIM]){
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           result[i][j]=0;
           for (int k=0; k<DIM; k++){
               result[i][j] = result[i][j] + (mat1[i][k] * mat2[k][j]);
           }
       }
    }
}

void MatrixScalarMultiply(float x, float mat[DIM][DIM], float result[DIM][DIM]){
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           result[i][j] = mat[i][j] * x;
       }
    }
}

float MatrixTrace(float mat[DIM][DIM]){
    float result = 0;
    for (int i=0; i<DIM; i++){
        result = result + mat[i][i];
       }
    return result;
}

void MatrixTranspose(float mat[DIM][DIM], float result[DIM][DIM]){
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           result[j][i] = mat[i][j];
       }
    }
}


void MatrixSubmatrix(int i, int j, float mat[DIM][DIM], float result[DIM-1][DIM-1]){
    int a = 0;
    int b = 0;
    for (int x=0; x<DIM; x++){
       for (int y=0; y<DIM; y++){
           if (i==x){
               x++;
           }
           if (j==y){
               y++;
           }
           result[a][b] = mat[x][y];
           if (b==1){
               b = 0;
               a++;
           }
           else{
               b++;
           }
       }
    }
}

float MatrixDeterminant(float mat[DIM][DIM]){
    float result = 0;
    float sub0[DIM-1][DIM-1];
    float sub1[DIM-1][DIM-1];
    float sub2[DIM-1][DIM-1];
    MatrixSubmatrix(0,0, mat, sub0);
    MatrixSubmatrix(0,1, mat, sub1);
    MatrixSubmatrix(0,2, mat, sub2);
    result = mat[0][0] * ((sub0[0][0] * sub0[1][1]) - (sub0[1][0] * sub0[0][1])) - mat[0][1] * ((sub1[0][0] * sub1[1][1]) - (sub1[1][0] * sub1[0][1])) + mat[0][2] * ((sub2[0][0] * sub2[1][1]) - (sub2[1][0] * sub2[0][1]));
    return result;
}

void getCofactor(float mat[DIM][DIM], float result[DIM][DIM]){
    float sub[DIM-1][DIM-1];
    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            /*MatrixSubmatrix(i, j, mat, sub);
            if((i+j) % 2 == 0){
                result[i][j] = ((sub[0][0]*sub[1][1]) - (sub[0][1]*sub[1][0]));
            }
            else{
                result[i][j] = ((sub[0][0]*sub[1][1]) - (sub[0][1]*sub[1][0])) * -1;
            }*/
            //if this were uncommented it would cause stack smash. The issue is unknown. Plz help! Feedback appreciated if debugged successfully.
        }
    }
}

void MatrixInverse(float mat[DIM][DIM], float result[DIM][DIM]){
    // get Cofactor
    float comat[DIM][DIM] = {{},{},{}};
    getCofactor(mat, comat);
    // transpose Cofactor
    float transposematrix[DIM][DIM] = {{},{},{}};
    MatrixTranspose(comat, transposematrix);
    //determinant
    float detmat = MatrixDeterminant(mat);
    // inverse
    float invdet = 1/detmat;
    MatrixScalarMultiply(invdet, transposematrix, result);          
}
    


int MatrixEquals(float mat1[DIM][DIM], float mat2[DIM][DIM]){
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           if (fabsf(mat1[i][j] - mat2[i][j]) > FP_DELTA){
               return FALSE;
           }
       }
    }
return TRUE;
}

void MatrixPrint(float mat[DIM][DIM]){
    printf(" --------------------\n");
    for (int i=0; i<DIM; i++){
       for (int j=0; j<DIM; j++){
           printf("| %0.2f ", mat[i][j]);
           if (j == 2){
               printf("|\n --------------------\n");
           }    
       }
    }
}


int MatrixEquals2(float mat1[DIM-1][DIM-1], float mat2[DIM-1][DIM-1]){
    for (int i=0; i<DIM-1; i++){
       for (int j=0; j<DIM-1; j++){
           if (fabsf(mat1[i][j] - mat2[i][j]) > FP_DELTA){
               return FALSE;
           }
       }
    }
return TRUE;
}



