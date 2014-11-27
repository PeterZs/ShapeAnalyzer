//
//  PetscLaplaceBeltramiOperator.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.11.14.
//
//

#include "PetscLaplaceBeltramiOperator.h"

void laplaceBeltrami::PetscLaplaceBeltramiOperator::getEigenfunctionMatrix(Mat *Phi) {
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        Vec phi;
        getEigenfunction(i, &phi);
        
        PetscHelper::setColumn(*Phi, phi, i);
        
        VecDestroy(&phi);
    }
    
    MatAssemblyBegin(*Phi, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(*Phi, MAT_FINAL_ASSEMBLY);
}
