//
//  PetscHeatDiffusion.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "PetscHeatDiffusion.h"

PetscHeatDiffusion::PetscHeatDiffusion(Shape* shape, PetscLaplaceBeltramiOperator* laplacian, vtkSmartPointer<vtkDoubleArray> initialCondition) : HeatDiffusion(shape, initialCondition), laplacian_(laplacian) {

    
    Mat Phi;
    PetscInt numberOfPoints = shape->getPolyData()->GetNumberOfPoints();
    MatCreateSeqDense(MPI_COMM_SELF, numberOfPoints, laplacian_->getNumberOfEigenfunctions(), NULL, &Phi);
    laplacian_->getEigenfunctionMatrix(&Phi);
    
    Mat PhiT;
    MatTranspose(Phi, MAT_INITIAL_MATRIX, &PhiT);
    MatDestroy(&Phi);
    
    Mat PhiTM;
    
    //get reference to mass matrix.
    Mat* M = laplacian_->getMassMatrix();
    MatMatMult(PhiT, *M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &PhiTM);
    MatDestroy(&PhiT);
    
    Vec u0;
    MatGetVecs(PhiTM, &u0, &PhiTMu0_);
    
    PetscHelper::vtkDoubleArrayToPetscVec(initialCondition, u0);
    
    MatMult(PhiTM, u0, PhiTMu0_);

    MatDestroy(&PhiTM);
    VecDestroy(&u0);
}

PetscHeatDiffusion::~PetscHeatDiffusion() {
    VecDestroy(&PhiTMu0_);
}

vtkSmartPointer<vtkDoubleArray> PetscHeatDiffusion::getHeat(double t) {
    PetscInt m = shape_->getPolyData()->GetNumberOfPoints();
    
    Vec utv;
    VecCreateSeq(PETSC_COMM_SELF, m, &utv);
    VecSet(utv, 0.0);
    
    for(PetscInt i = 0; i < laplacian_->getNumberOfEigenfunctions(); i++) {
        Vec phi;
        PetscScalar lambda;
        laplacian_->getEigenpair(i, &phi, &lambda);
        PetscScalar y;
        VecGetValues(PhiTMu0_, 1, &i, &y);
        
        //Y = aX + Y (ut = exp(lambda * t) * y * phi + ut)
        VecAXPY(utv, exp(lambda * t) * y, phi);
        
        VecDestroy(&phi);
    }
    
    vtkSmartPointer<vtkDoubleArray> ut = PetscHelper::petscVecToVtkDoubleArray(utv);

    VecDestroy(&utv);
    
    return ut;
}
