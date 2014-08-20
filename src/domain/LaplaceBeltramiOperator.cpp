//
//  LaplaceBeltramiOperator.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#include "LaplaceBeltramiOperator.h"

void LaplaceBeltramiOperator::initialize(Shape* shape, int numberOfEigenfunctions) {
    shape_ = shape;
    numberOfEigenfunctions_ = numberOfEigenfunctions;
}

void LaplaceBeltramiOperator::getEigenfunctionMatrix(Mat *Phi) {
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        Vec phi;
        getEigenfunction(i, &phi);
        
        PetscHelper::setColumn(*Phi, phi, i);
        
        VecDestroy(&phi);
    }
    
    
    MatAssemblyBegin(*Phi, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(*Phi, MAT_FINAL_ASSEMBLY);
}