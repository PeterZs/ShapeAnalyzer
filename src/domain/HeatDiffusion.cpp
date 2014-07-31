//
//  HeatDiffusion.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "HeatDiffusion.h"

HeatDiffusion::HeatDiffusion(Shape* shape, FEMLaplaceBeltramiOperator* laplacian) : shape_(shape), laplacian_(laplacian) {
    PetscInt n = laplacian_->getNumberOfEigenfunctions();
    PetscInt m = shape->getPolyData()->GetNumberOfPoints();
    
    MatCreateSeqDense(MPI_COMM_SELF, m, n, NULL, &Phi_);
    
    PetscInt* row = new PetscInt[m];
    
    for(PetscInt i = 0; i < n; i++) {
        row[i] = i;
    }
    
    for(PetscInt j = 0; j < m; j++) {

        PetscScalar* x;
        laplacian_->getEigenfunction(j, &x);
        

        MatSetValues(Phi_, m, row, 1, &j, x, INSERT_VALUES);
    }
    

    MatAssemblyBegin(Phi_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(Phi_, MAT_FINAL_ASSEMBLY);
}