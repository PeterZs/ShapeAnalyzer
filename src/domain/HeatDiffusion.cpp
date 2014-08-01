//
//  HeatDiffusion.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "HeatDiffusion.h"

HeatDiffusion::HeatDiffusion(Shape* shape, FEMLaplaceBeltramiOperator* laplacian, ScalarPointAttribute& initialCondition) : shape_(shape), laplacian_(laplacian) {
    PetscInt n = laplacian_->getNumberOfEigenfunctions();
    PetscInt m = shape->getPolyData()->GetNumberOfPoints();
    
    Mat PhiT;
    MatCreateSeqDense(MPI_COMM_SELF, n, m, NULL, &PhiT);
    
    PetscInt* column = new PetscInt[m];
    
    for(PetscInt j = 0; j < m; j++) {
        column[j] = j;
    }
    
    for(PetscInt i = 0; i < n; i++) {
        PetscScalar* x;
        laplacian_->getEigenfunction(i, &x);
        
        MatSetValues(PhiT, 1, &i, m, column, x, INSERT_VALUES);
    }
    

    MatAssemblyBegin(PhiT, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(PhiT, MAT_FINAL_ASSEMBLY);
    
    
    
    Mat PhiTM;
    Mat M;
    laplacian_->getMassMatrix(&M);
    MatMatMult(PhiT, M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &PhiTM);
    Vec u0;
    MatGetVecs(PhiTM, &u0, &PhiTMu0_);
    
    for(vtkIdType i = 0; i < shape->getPolyData()->GetNumberOfPoints(); i++) {
        VecSetValue(u0, i, initialCondition.getScalars()->GetValue(i), INSERT_VALUES);
    }
    VecAssemblyBegin(u0);
    VecAssemblyEnd(u0);
    
    MatMult(PhiTM, u0, PhiTMu0_);
    
    MatDestroy(&PhiT);
    MatDestroy(&PhiTM);
    VecDestroy(&u0);
}

HeatDiffusion::~HeatDiffusion() {
    VecDestroy(&PhiTMu0_);
}

void HeatDiffusion::getHeat(ScalarPointAttribute& heat, double t) {
    PetscInt n = laplacian_->getNumberOfEigenfunctions();
    PetscInt m = shape_->getPolyData()->GetNumberOfPoints();
    
    Vec ut;
    VecCreateSeq(PETSC_COMM_SELF, m, &ut);
    VecSet(ut, 0.0);
    
    for(PetscInt i = 0; i < n; i++) {
        Vec phi;
        laplacian_->getEigenfunction(i, &phi);
        PetscScalar* h;
        VecGetArray(phi, &h);
        PetscScalar y;
        VecGetValues(PhiTMu0_, 1, &i, &y);
        
        //Y = aX + Y (ut = exp(laplacian_->getEigenvalue(i) * t) * y * phi + ut)
        VecAXPY(ut, exp(laplacian_->getEigenvalue(i) * t) * y, phi);
    }
    
    PetscScalar* h;
    VecGetArray(ut, &h);
    for(PetscInt j = 0; j < m; j++) {
        heat.getScalars()->SetValue(j, h[j]);
    }
    VecDestroy(&ut);
}
