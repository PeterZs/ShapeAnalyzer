//
//  WaveKernelSignature.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#include "WaveKernelSignature.h"

#include <limits>


signature::WaveKernelSignature::WaveKernelSignature(Shape* shape, int dimension, LaplaceBeltramiOperator* laplacian, double wksVariance) : LaplaceBeltramiSignature(shape, dimension, laplacian), wksVariance_(wksVariance) {

    PetscScalar* logLambda = new PetscScalar[laplacian_->getNumberOfEigenfunctions()];
    
    double maximum = -std::numeric_limits<double>::infinity();
    for(PetscInt i = 0; i < laplacian_->getNumberOfEigenfunctions(); i++) {
        PetscScalar lambda = laplacian_->getEigenvalue(i);
        logLambda[i]= log(std::max((abs(lambda)), 1e-6));
        if(maximum < logLambda[i]) {
            maximum = logLambda[i];
        }
    }
    
    
    PetscScalar step = (maximum / 1.02 - logLambda[1]) / (dimension_ - 1);
    PetscScalar* e = new PetscScalar[dimension_];

    for(PetscInt i = 0; i < dimension_; i++) {
        e[i] = logLambda[1] + i*step;
    }
    
    PetscScalar sigma = (e[1] - e[0]) * wksVariance_;

    for(PetscInt i = 0; i < dimension_; i++) {
        Vec wksi; //i-th component of wks
        VecCreateSeq(MPI_COMM_SELF, shape_->getPolyData()->GetNumberOfPoints(), &wksi);
        VecSet(wksi, 0.0);
        PetscScalar C = 0;
        for(PetscInt k = 0; k < laplacian_->getNumberOfEigenfunctions(); k++) {
            Vec phi;
            laplacian_->getEigenfunction(k, &phi);
//VecPow was introduced in Petsc ver. 3.5.0
//Ubuntu 14.04 only has only 3.4.8 so check version here.
#if PETSC_VERSION_GE(3,5,0)
            VecPow(phi, 2.0);
#else
            //we can subsitute VecPow(phi,2) by VecPointwiseMult(phi,phi,phi)
            VecPointwiseMult(phi,phi,phi);
#endif
            
            PetscScalar c = exp( -pow(e[i] - logLambda[k], 2.0) / ( 2.0 * sigma * sigma ));
            VecAXPY(wksi, c, phi);
            C += c;
            
            
            VecDestroy(&phi);
        }
        
        VecScale(wksi, C);
        
        
        PetscHelper::setRow(signature_, wksi, i);
        
        VecDestroy(&wksi);
    }
    MatAssemblyBegin(signature_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(signature_, MAT_FINAL_ASSEMBLY);
    
    delete [] logLambda;
    delete [] e;
    
}