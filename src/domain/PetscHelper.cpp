//
//  PetscHelper.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 04.08.14.
//
//

#include "PetscHelper.h"

void PetscHelper::scalarPointAttributeToPetscVec(ScalarPointAttribute &attr, Vec &vec) {
    PetscInt size = attr.getShape()->getPolyData()->GetNumberOfPoints();
    
    for(PetscInt i = 0; i < size; i++) {
        VecSetValue(vec, i, attr.getScalars()->GetValue(i), INSERT_VALUES);
    }
    
    VecAssemblyBegin(vec);
    VecAssemblyEnd(vec);
}

void PetscHelper::petscVecToScalarPointAttribute(Vec& vec, ScalarPointAttribute& attr) {
    PetscScalar* arr;
    VecGetArray(vec, &arr);
    for(PetscInt j = 0; j < attr.getShape()->getPolyData()->GetNumberOfPoints(); j++) {
        attr.getScalars()->SetValue(j, arr[j]);
    }
}


void PetscHelper::setRow(Mat &A, Vec &ai, PetscInt i) {
    PetscInt size;
    VecGetSize(ai, &size);
    PetscInt* colIdx = new PetscInt[size];
    
    PetscScalar* row;
    VecGetArray(ai, &row);
    
    for(PetscInt j = 0; j < size; j++) {
        colIdx[j] = j;
    }
    
    MatSetValues(A, 1, &i, size, colIdx, row, INSERT_VALUES);
}

void PetscHelper::setColumn(Mat &A, Vec &ai, PetscInt i) {
    PetscInt size;
    VecGetSize(ai, &size);
    PetscInt* rowIdx = new PetscInt[size];
    
    PetscScalar* column;
    VecGetArray(ai, &column);
    
    for(PetscInt j = 0; j < size; j++) {
        rowIdx[j] = j;
    }
    
    MatSetValues(A, size, rowIdx, 1, &i, column, INSERT_VALUES);
}


