//
//  HeatDiffusion.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "HeatDiffusion.h"

HeatDiffusion::HeatDiffusion(Shape* shape, ScalarPointAttribute& initialCondition, int numberOfEigenfunctions) : shape_(shape), numberOfEigenfunctions_(numberOfEigenfunctions) {

    
    Mat Phi;
    PetscInt numberOfPoints = shape->getPolyData()->GetNumberOfPoints();
    MatCreateSeqDense(MPI_COMM_SELF, numberOfPoints, numberOfEigenfunctions_, NULL, &Phi);
    shape_->getLaplacian(numberOfEigenfunctions_)->getEigenfunctionMatrix(&Phi);
    
    Mat PhiT;
    MatTranspose(Phi, MAT_INITIAL_MATRIX, &PhiT);
    MatDestroy(&Phi);
    
    Mat PhiTM;
    
    //get reference to mass matrix.
    Mat* M = shape_->getLaplacian(numberOfEigenfunctions_)->getMassMatrix();
    MatMatMult(PhiT, *M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &PhiTM);
    MatDestroy(&PhiT);
    
    Vec u0;
    MatGetVecs(PhiTM, &u0, &PhiTMu0_);
    
    ScalarPointAttribute::scalarPointAttributeToPetscVec(initialCondition, u0);
    
    MatMult(PhiTM, u0, PhiTMu0_);

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
        PetscScalar lambda;
        shape_->getLaplacian(numberOfEigenfunctions_)->getEigenpair(i, &phi, &lambda);
        PetscScalar y;
        VecGetValues(PhiTMu0_, 1, &i, &y);
        
        //Y = aX + Y (ut = exp(lambda * t) * y * phi + ut)
        VecAXPY(ut, exp(lambda * t) * y, phi);
        
        VecDestroy(&phi);
    }
    
    ScalarPointAttribute::petscVecToScalarPointAttribute(ut, heat);

    VecDestroy(&ut);
}
