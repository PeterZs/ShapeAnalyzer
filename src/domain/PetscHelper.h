//
//  PetscHelper.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 04.08.14.
//
//

#ifndef __ShapeAnalyzer__PetscHelper__
#define __ShapeAnalyzer__PetscHelper__

#include "attributes/ScalarPointAttribute.h"

#include <slepceps.h>

class ScalarPointAttribute;

class PetscHelper {
public:
    //convert scalar point attribute to vector
    static void scalarPointAttributeToPetscVec(ScalarPointAttribute& attr, Vec& vec);

    //convert scalar point attribute to vector
    static void petscVecToScalarPointAttribute(Vec& vec, ScalarPointAttribute& attr);
    
    
    //set i-th row in matrix
    static void setRow(Mat& A, Vec& ai, PetscInt i);

    //set i-th column in matrix
    static void setColumn(Mat& A, Vec& ai, PetscInt i);
};

#endif /* defined(__ShapeAnalyzer__PetscHelper__) */
