//
//  PetscHelper.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 04.08.14.
//
//

#ifndef __ShapeAnalyzer__PetscHelper__
#define __ShapeAnalyzer__PetscHelper__

#include <slepceps.h>

class PetscHelper {
public:
    //set block B in matrix A. Element A(i, j) will be set to B(0,0) and so on...
    static void setBlock(Mat& A, Mat& B, PetscInt i, PetscInt j);
    
    //set i-th row in matrix A
    static void setRow(Mat& A, Vec& ai, PetscInt i);

    //set i-th column in matrix A
    static void setColumn(Mat& A, Vec& ai, PetscInt i);
    
    //reshape n*m-dimensional vector b as matrix B with m rows and n columns. Elements are filled in row wise into matrix B.
    static void reshape(Mat& B, Vec& b, PetscInt m, PetscInt n);
};

#endif /* defined(__ShapeAnalyzer__PetscHelper__) */
