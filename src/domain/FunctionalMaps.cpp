//
//  FunctionalMaps.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 02.08.14.
//
//

#include "FunctionalMaps.h"

FunctionalMaps::FunctionalMaps(Shape& shape1, Shape& shape2, vector<ScalarPointAttribute>& c1, vector<ScalarPointAttribute>& c2, int numberOfEigenfunctions) : shape1_(shape1), shape2_(shape2), c1_(c1), c2_(c2), numberOfEigenfunctions_(numberOfEigenfunctions) {
    
    numberOfConstraints_ = c1_.size();

    //compute Phi_M^T * M_M and Phi_N^T * M_N
    setupPhiTM(shape1_, &Phi1_, &PhiTM1_);
    setupPhiTM(shape2_, &Phi2_, &PhiTM2_);
    

    // compute A^T which corresponds to all the constraits ci1 on shape M (shape1) and vec bi which corresponds to contraint ci2 on shape N (shape2)
    Mat AT;
    Mat B;
    
    MatCreateSeqDense(MPI_COMM_SELF, numberOfConstraints_, numberOfEigenfunctions_, NULL, &AT);
    MatCreateSeqDense(MPI_COMM_SELF, numberOfEigenfunctions_, numberOfConstraints_, NULL, &B);
    
    for(PetscInt i = 0; i < numberOfConstraints_; i++) {
        Vec ci1; // Vector representing the function ci_1 defined on the vertices of shape1
        Vec ai; // eigenbasis coefficients of contraint function ci_1
        
        //get vectors ci1, ai which can be multiplied by PhiTM1 (ci1) and that the matrix vector product PhiTM * ci1 can be stored in (ai)
        MatGetVecs(PhiTM1_, &ci1, &ai);
        
        //copy contraint c1_ which is of type scalar point attribute into Petsc vector
        ScalarPointAttribute::scalarPointAttributeToPetscVec(c1_[i], ci1);
        
        
        //compute i-th row (ai) of A^T
        MatMult(PhiTM1_, ci1, ai);
        
        PetscHelper::setRow(AT, ai, i);
        
        
        //set b_i which is an array of Vecs (corresponds to i-th column of B)
        Vec ci2;
        Vec bi;
        MatGetVecs(PhiTM2_, &ci2, &bi);
        ScalarPointAttribute::scalarPointAttributeToPetscVec(c2_[i], ci2);
        
        MatMult(PhiTM2_, ci2, bi);
        
        PetscHelper::setColumn(B, bi, i);
        
        VecDestroy(&ci1);
        VecDestroy(&ci2);
        VecDestroy(&ai);
        VecDestroy(&bi);
    }
    
    
    MatAssemblyBegin(AT, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(AT, MAT_FINAL_ASSEMBLY);
    
    MatAssemblyBegin(B, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(B, MAT_FINAL_ASSEMBLY);


    MatCreateSeqAIJ(MPI_COMM_SELF, numberOfConstraints_*numberOfEigenfunctions_, numberOfEigenfunctions_*numberOfEigenfunctions_, numberOfEigenfunctions_, NULL, &AT_);
    VecCreateSeq(PETSC_COMM_SELF, numberOfConstraints_*numberOfEigenfunctions_, &B_);
   
    PetscInt* rowIdx = new PetscInt[numberOfConstraints_];
    
    for(PetscInt j = 0; j < numberOfEigenfunctions_; j++) {
        PetscHelper::setBlock(AT_, AT, j*numberOfConstraints_, j*numberOfEigenfunctions_);
        
        const PetscScalar* row;
        MatGetRow(B, j, NULL, NULL, &row);

        for(PetscInt i = 0; i < numberOfConstraints_; i++) {
            rowIdx[i] = j*numberOfConstraints_+i;
        }
        VecSetValues(B_, numberOfConstraints_, rowIdx, row, INSERT_VALUES);

    }
    MatAssemblyBegin(AT_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(AT_, MAT_FINAL_ASSEMBLY);
    
    VecAssemblyBegin(B_);
    VecAssemblyEnd(B_);
    
    delete [] rowIdx;
    //MatDestroy(&AT);
    //MatDestroy(&B);
    


    

    //compute C
    
    //setup least squares solver
    PetscErrorCode ierr;
    ierr = KSPCreate(PETSC_COMM_SELF, &ksp_);
    

    ierr = KSPSetOperators(ksp_, AT_, AT_);
    ierr = KSPSetType(ksp_, KSPLSQR);
    PC pc;
    ierr = KSPGetPC(ksp_, &pc);
    PCSetType(pc, PCNONE);
    

    
    
    //Solve for C
    Vec C;
    ierr = VecCreateSeq(PETSC_COMM_SELF, numberOfEigenfunctions_*numberOfEigenfunctions_, &C);
    ierr = KSPSolve(ksp_, B_, C);
    
    PetscScalar residual;
    KSPGetResidualNorm(ksp_, &residual);
    cout << "Residual "<<residual<<endl;
    
    MatCreateSeqDense(PETSC_COMM_SELF, numberOfEigenfunctions_, numberOfEigenfunctions_, NULL, &C_);
    
    PetscHelper::reshape(C_, C, numberOfEigenfunctions_, numberOfEigenfunctions_);
    //MatTranspose(C_, MAT_REUSE_MATRIX, &C_);
    
    VecDestroy(&C);

    
    MatView(C_, PETSC_VIEWER_STDOUT_SELF);
    
    Mat A;
    MatTranspose(AT, MAT_INITIAL_MATRIX, &A);
    Mat CA;
    
    MatMatMult(C_, A, MAT_INITIAL_MATRIX, PETSC_DEFAULT, &CA);
    

    MatAXPY(CA, -1.0, B, SAME_NONZERO_PATTERN);
    
    PetscReal res;
    MatNorm(CA, NORM_FROBENIUS, &res);
    cout << "residual "<< res <<endl;
    
    MatAXPY(A, -1.0, B, SAME_NONZERO_PATTERN);

    MatNorm(A, NORM_FROBENIUS, &res);
    cout << "residual "<< res <<endl;
}

FunctionalMaps::~FunctionalMaps() {
    MatDestroy(&C_);
    MatDestroy(&AT_);
    MatDestroy(&PhiTM1_);
    MatDestroy(&PhiTM2_);
    VecDestroy(&B_);
    
    MatDestroy(&Phi1_);
    MatDestroy(&Phi2_);
    
    
    KSPDestroy(&ksp_);
}

void FunctionalMaps::transferFunction(ScalarPointAttribute &f1, ScalarPointAttribute &f2) {
    Vec f1v;
    VecCreateSeq(PETSC_COMM_SELF, f1.getShape()->getPolyData()->GetNumberOfPoints(), &f1v);
    ScalarPointAttribute::scalarPointAttributeToPetscVec(f1, f1v);
    
    Vec a;
    MatGetVecs(PhiTM1_, NULL, &a);
    MatMult(PhiTM1_, f1v, a);
    
    
    Vec b;
    MatGetVecs(C_, NULL, &b);
    MatMult(C_, a, b);
    
    Vec f2v;
    MatGetVecs(Phi1_, NULL, &f2v);
    
    MatMult(Phi1_, b, f2v);
    ScalarPointAttribute::petscVecToScalarPointAttribute(f2v, f2);
    
    
    VecDestroy(&f1v);
    VecDestroy(&f2v);
    VecDestroy(&a);
    VecDestroy(&b);
}

void FunctionalMaps::setupPhiTM(Shape& shape, Mat* Phi, Mat *PhiTM) {
    PetscInt numberOfPoints = shape.getPolyData()->GetNumberOfPoints();
    
    MatCreateSeqDense(MPI_COMM_SELF, numberOfPoints, numberOfEigenfunctions_, NULL, Phi);
    shape.getLaplacian(numberOfEigenfunctions_)->getEigenfunctionMatrix(Phi);
    
    Mat PhiT;
    MatTranspose(*Phi, MAT_INITIAL_MATRIX, &PhiT);
    
    //get reference to mass matrix
    Mat* M = shape.getLaplacian(numberOfEigenfunctions_)->getMassMatrix();
    MatMatMult(PhiT, *M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, PhiTM);
    
    MatDestroy(&PhiT);
}


