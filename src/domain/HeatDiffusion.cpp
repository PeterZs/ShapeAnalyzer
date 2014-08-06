//
//  HeatDiffusion.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "HeatDiffusion.h"

HeatDiffusion::HeatDiffusion(Shape* shape, ScalarPointAttribute& initialCondition, int numberOfEigenfunctions) : shape_(shape), numberOfEigenfunctions_(numberOfEigenfunctions) {
    PetscInt m = shape->getPolyData()->GetNumberOfPoints();
    
    Mat PhiT;
    MatCreateSeqDense(MPI_COMM_SELF, numberOfEigenfunctions_, m, NULL, &PhiT);
    
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        Vec phi;
        shape_->getLaplacian(numberOfEigenfunctions_)->getEigenfunction(i, &phi);
        
        PetscHelper::setRow(PhiT, phi, i);
    }
    

    MatAssemblyBegin(PhiT, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(PhiT, MAT_FINAL_ASSEMBLY);
    
    
    
    Mat PhiTM;
    Mat M;
    shape_->getLaplacian(numberOfEigenfunctions_)->getMassMatrix(&M);
    MatMatMult(PhiT, M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &PhiTM);
    Vec u0;
    MatGetVecs(PhiTM, &u0, &PhiTMu0_);
    
    PetscHelper::scalarPointAttributeToPetscVec(initialCondition, u0);
    
    MatMult(PhiTM, u0, PhiTMu0_);
    
    MatDestroy(&PhiT);
    MatDestroy(&PhiTM);
    VecDestroy(&u0);
}

HeatDiffusion::~HeatDiffusion() {
    VecDestroy(&PhiTMu0_);
}

void HeatDiffusion::getHeat(ScalarPointAttribute& heat, double t) {
    PetscInt m = shape_->getPolyData()->GetNumberOfPoints();
    
    Vec ut;
    VecCreateSeq(PETSC_COMM_SELF, m, &ut);
    VecSet(ut, 0.0);
    
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        Vec phi;
        shape_->getLaplacian(numberOfEigenfunctions_)->getEigenfunction(i, &phi);
        PetscScalar y;
        VecGetValues(PhiTMu0_, 1, &i, &y);
        
        //Y = aX + Y (ut = exp(laplacian_->getEigenvalue(i) * t) * y * phi + ut)
        VecAXPY(ut, exp(shape_->getLaplacian(numberOfEigenfunctions_)->getEigenvalue(i) * t) * y, phi);
    }
    
    PetscHelper::petscVecToScalarPointAttribute(ut, heat);

    VecDestroy(&ut);
}
