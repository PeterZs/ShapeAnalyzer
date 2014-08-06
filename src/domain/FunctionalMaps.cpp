//
//  FunctionalMaps.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 02.08.14.
//
//

#include "FunctionalMaps.h"

FunctionalMaps::~FunctionalMaps() {
    MatDestroy(&C_);
    MatDestroy(&AT_);
    MatDestroy(&PhiTM1_);
    MatDestroy(&PhiTM2_);
    MatDestroy(&B_);
    
    
    KSPDestroy(&ksp_);
}

void FunctionalMaps::initialize() {
    //compute Phi_M^T * M_M and Phi_N^T * M_N
    setupPhiTM(shape1_, &Phi1_, &PhiTM1_);
    setupPhiTM(shape2_, &Phi2_, &PhiTM2_);
    

    // compute A^T which corresponds to all the constraits ci1 on shape M (shape1) and vec bi which corresponds to contraint ci2 on shape N (shape2)
    MatCreateSeqDense(MPI_COMM_SELF, numberOfConstraints_, numberOfEigenfunctions_, NULL, &AT_);
    
    MatCreateSeqDense(MPI_COMM_SELF, numberOfEigenfunctions_, numberOfConstraints_, NULL, &B_);
    
    for(int i = 0; i < numberOfConstraints_; i++) {
        Vec ci1;
        Vec ai;
        
        //get vectors ci1, ai which can be multiplied by PhiTM1 (ci1) and that the matrix vector product PhiTM * ci1 can be stored in (ai)
        MatGetVecs(PhiTM1_, &ci1, &ai);
        
        //copy contraint c1_ which is of type scalar point attribute into Petsc vector
        PetscHelper::scalarPointAttributeToPetscVec(c1_[i], ci1);
        
        
        //compute i-th row (ai) of A^T
        MatMult(PhiTM1_, ci1, ai);
        
        PetscHelper::setRow(AT_, ai, i);

        
        //set b_i which is an array of Vecs (corresponds to i-th column of B)
        Vec ci2;
        Vec bi;
        MatGetVecs(PhiTM2_, &ci2, &bi);
        PetscHelper::scalarPointAttributeToPetscVec(c2_[i], ci2);
        
        MatMult(PhiTM2_, ci2, bi);
        
        PetscHelper::setColumn(B_, bi, i);
        
        VecDestroy(&ci1);
        VecDestroy(&ci2);
        VecDestroy(&ai);
        VecDestroy(&bi);
    }
    
    
    MatAssemblyBegin(AT_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(AT_, MAT_FINAL_ASSEMBLY);
    
    MatAssemblyBegin(B_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(B_, MAT_FINAL_ASSEMBLY);
    

    //compute C
    
    //setup least squares solver
    PetscErrorCode ierr;
    ierr = KSPCreate(PETSC_COMM_SELF, &ksp_);
    ierr = KSPSetOperators(ksp_, AT_, AT_);
    ierr = KSPSetType(ksp_, KSPLSQR);
    
    
    MatCreateSeqDense(PETSC_COMM_SELF, numberOfEigenfunctions_, numberOfEigenfunctions_, NULL, &C_);
    

    //Solve for i-th row of C
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        Vec ci;
        
        ierr = VecCreateSeq(PETSC_COMM_SELF, numberOfEigenfunctions_, &ci);
        
        
        const PetscScalar* row;
        MatGetRow(B_, i, NULL, NULL, &row);
        Vec bi;
        VecCreateSeq(PETSC_COMM_SELF, numberOfConstraints_, &bi);
        PetscInt* rowIdx = new PetscInt[numberOfConstraints_];

        for(int j = 0; j < numberOfConstraints_; j++) {
            rowIdx[j] = j;
        }
        VecSetValues(bi, numberOfConstraints_, rowIdx, row, INSERT_VALUES);
        VecAssemblyBegin(bi);
        VecAssemblyEnd(bi);
        delete [] rowIdx;
        
        ierr = KSPSolve(ksp_, bi, ci);
        
        
        
        PetscHelper::setRow(C_, ci, i);
        
        Vec test;
        VecCreateSeq(PETSC_COMM_SELF, numberOfConstraints_, &test);
        MatMult(AT_, ci, test);
        VecAXPY(bi,-1.0, test);
        PetscScalar bla;
        VecNorm(bi, NORM_2, &bla);
        cout << "Vecnorm  "<<bla<<endl;
        
    
        VecDestroy(&ci);
    }
    
    MatAssemblyBegin(C_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(C_, MAT_FINAL_ASSEMBLY);
    
    MatView(C_, PETSC_VIEWER_STDOUT_SELF);
    
    Mat A;
    MatTranspose(AT_, MAT_INITIAL_MATRIX, &A);
    Mat CA;
    
    MatMatMult(C_, A, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &CA);
    

    MatAXPY(CA, -1.0, B_, SAME_NONZERO_PATTERN);
    
    PetscReal res;
    MatNorm(CA, NORM_FROBENIUS, &res);
    cout << "residual "<< res <<endl;
    
    MatAXPY(A, -1.0, B_, SAME_NONZERO_PATTERN);

    MatNorm(A, NORM_FROBENIUS, &res);
    cout << "residual "<< res <<endl;
}

void FunctionalMaps::transferFunction(ScalarPointAttribute &f1, ScalarPointAttribute &f2) {
    Vec f1v;
    VecCreateSeq(PETSC_COMM_SELF, f1.getShape()->getPolyData()->GetNumberOfPoints(), &f1v);
    PetscHelper::scalarPointAttributeToPetscVec(f1, f1v);
    
    Vec a;
    MatGetVecs(PhiTM1_, NULL, &a);
    MatMult(PhiTM1_, f1v, a);
    
    
    Vec b;
    MatGetVecs(C_, NULL, &b);
    MatMult(C_, a, b);
    
    Vec f2v;
    MatGetVecs(Phi1_, NULL, &f2v);
    
    MatMult(Phi1_, b, f2v);
    PetscHelper::petscVecToScalarPointAttribute(f2v, f2);
    
    
    VecDestroy(&f1v);
    VecDestroy(&f2v);
    VecDestroy(&a);
    VecDestroy(&b);
}

void FunctionalMaps::setupPhiTM(Shape& shape, Mat* Phi, Mat *PhiTM) {
    PetscInt m = shape.getPolyData()->GetNumberOfPoints();
    
    Mat PhiT;
    MatCreateSeqDense(MPI_COMM_SELF, numberOfEigenfunctions_, m, NULL, &PhiT);
    
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        Vec phi;
        shape.getLaplacian(numberOfEigenfunctions_)->getEigenfunction(i, &phi);
        
        PetscHelper::setRow(PhiT, phi, i);
    }
    
    MatAssemblyBegin(PhiT, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(PhiT, MAT_FINAL_ASSEMBLY);
    
    
    MatTranspose(PhiT, MAT_INITIAL_MATRIX, Phi);
    
    Mat M;
    shape.getLaplacian(numberOfEigenfunctions_)->getMassMatrix(&M);
    MatMatMult(PhiT, M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, PhiTM);
    
    MatDestroy(&PhiT);
}


