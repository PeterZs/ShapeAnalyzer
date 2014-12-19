#include "PetscFunctionalMaps.h"

///////////////////////////////////////////////////////////////////////////////
PetscFunctionalMaps::PetscFunctionalMaps(shared_ptr<Shape> shape1, shared_ptr<Shape> shape2, shared_ptr<PetscLaplaceBeltramiOperator> laplacian1, shared_ptr<PetscLaplaceBeltramiOperator> laplacian2, vector<vtkSmartPointer<vtkDoubleArray>>& c1, vector<vtkSmartPointer<vtkDoubleArray>>& c2, int numberOfEigenfunctions, double alpha, double lambda, double mu) : FunctionalMaps(shape1, shape2, c1, c2, numberOfEigenfunctions), mu_(mu), lambda_(lambda), alpha_(alpha), laplacian1_(laplacian1), laplacian2_(laplacian2) {
    
    cout << "Initializing..."<<flush;
    
    //compute Phi_M^T * M_M and Phi_N^T * M_N
    setupPhiTM(shape1_.get(), laplacian1_.get(), &Phi1_, &PhiTM1_);
    setupPhiTM(shape2_.get(), laplacian2_.get(), &Phi2_, &PhiTM2_);
    
    
    // compute A which corresponds to all the constraits ci1 on shape M (shape1) and B which corresponds to contraints ci2 on shape N (shape2)
    MatCreateSeqDense(MPI_COMM_SELF, numberOfConstraints_, numberOfEigenfunctions_, NULL, &A_);
    MatCreateSeqDense(MPI_COMM_SELF, numberOfConstraints_, numberOfEigenfunctions_, NULL, &B_);
    
    for(PetscInt i = 0; i < numberOfConstraints_; i++) {
        // Vector representing the function ci_1 defined on the vertices of shape1
        Vec ci1;
        
        // eigenbasis coefficients of contraint function ci_1
        Vec ai;
        
        //get vectors ci1, ai which can be multiplied by PhiTM1 (ci1) and that the matrix vector product PhiTM * ci1 can be stored in (ai)
        MatGetVecs(PhiTM1_, &ci1, &ai);
        
        //copy contraint c1_ which is of type scalar point attribute into Petsc vector
        PetscHelper::vtkDataArrayToPetscVec(c1_[i], ci1);
        
        
        //compute i-th row (ai) of A^T
        MatMult(PhiTM1_, ci1, ai);
        
        PetscHelper::setRow(A_, ai, i);
        
        
        //set b_i which is an array of Vecs (corresponds to i-th column of B)
        Vec ci2;
        Vec bi;
        MatGetVecs(PhiTM2_, &ci2, &bi);
        PetscHelper::vtkDataArrayToPetscVec(c2_[i], ci2);
        
        MatMult(PhiTM2_, ci2, bi);
        
        PetscHelper::setRow(B_, bi, i);
        
        VecDestroy(&ci1);
        VecDestroy(&ci2);
        VecDestroy(&ai);
        VecDestroy(&bi);
    }
    
    
    MatAssemblyBegin(A_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A_, MAT_FINAL_ASSEMBLY);
    
    MatAssemblyBegin(B_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(B_, MAT_FINAL_ASSEMBLY);
    
    MatDuplicate(B_, MAT_COPY_VALUES, &O_);
    
    MatCreateSeqDense(MPI_COMM_SELF, numberOfEigenfunctions_, numberOfEigenfunctions_, NULL, &C_);
    MatZeroEntries(C_);

    // create and matrices T, C and O
    Mat T;
    Mat O;
    Mat C;
    MatCreateSeqDense(MPI_COMM_SELF, numberOfEigenfunctions_, numberOfEigenfunctions_, NULL, &C);
    MatCreateSeqDense(MPI_COMM_SELF, numberOfConstraints_, numberOfEigenfunctions_, NULL, &T);
    MatCreateSeqDense(MPI_COMM_SELF, numberOfConstraints_, numberOfEigenfunctions_, NULL, &O);

    cout << " done."<<endl;
    cout << "Computing C..."<<endl;
    for(int i = 0; i < 2700; i++) {
        cout << "    > Iteration "<<i<<"..."<<flush;
        // compute grad_C
        // C = A*C_k, O = A*C_k
        MatMatMult(A_, C_, MAT_REUSE_MATRIX, PETSC_DEFAULT, &T);
        //MatCopy(T, O, SAME_NONZERO_PATTERN);
        // T = 1.0 * O_ + T
        //MatAXPY(T, 1.0, O_, SAME_NONZERO_PATTERN);
        // T = -1.0 * B_ + T
        MatAXPY(T, -1.0, B_, SAME_NONZERO_PATTERN);
        // C = A^T*T
        MatTransposeMatMult(A_, T, MAT_REUSE_MATRIX, PETSC_DEFAULT, &C);
        
        //update C
        //C = -1/alpha * C + C_k
        MatAYPX(C, -1.0 / alpha_, C_, SAME_NONZERO_PATTERN);

        // compute grad_O
        // O = 1.0 * O_k + O
        //MatAXPY(O, 1.0, O_, SAME_NONZERO_PATTERN);
        // O = -1.0 * B_ + O
        //MatAXPY(O, -1.0, B_, SAME_NONZERO_PATTERN);

        //update O
        //O = -1/alpha * O + O_k
        //MatAYPX(O, -1.0 / alpha_, O_, SAME_NONZERO_PATTERN);
        
        // apply proximity operators
        proxOperator1(&C);
        //proxOperator2(&O);
        
        cout << " done."<<endl;
    }
    
    MatView(C_, PETSC_VIEWER_STDOUT_SELF);
    
    MatView(O_, PETSC_VIEWER_STDOUT_SELF);
}


///////////////////////////////////////////////////////////////////////////////
void PetscFunctionalMaps::proxOperator1(Mat* C) {
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        for(PetscInt j = 0; j < numberOfEigenfunctions_; j++) {
            PetscScalar w = pow((double) (i - j), 2.0);
            PetscScalar c;
            MatGetValue(*C, i, j, &c);
            PetscScalar z = ( lambda_ / alpha_ ) * w;
            if(c < - z) {
                MatSetValue(C_, i, j, c + z, INSERT_VALUES);
            } else if(c > z) {
                MatSetValue(C_, i, j, c - z, INSERT_VALUES);
            } else {
                MatSetValue(C_, i, j, 0.0, INSERT_VALUES);
            }
            
            
        }
    }
    MatAssemblyBegin(C_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(C_, MAT_FINAL_ASSEMBLY);
}


///////////////////////////////////////////////////////////////////////////////
void PetscFunctionalMaps::proxOperator2(Mat* O) {
    PetscInt a, b;
    MatGetSize(*O, &a, &b);
    for(PetscInt i = 0; i < numberOfConstraints_; i++) {
        Vec oi;
        VecCreateSeq(PETSC_COMM_SELF, numberOfEigenfunctions_, &oi);
        PetscHelper::getRow(oi, *O, i);
        PetscScalar norm;
        VecNorm(oi, NORM_2, &norm);
        PetscScalar scalar = max(norm - mu_ / alpha_, 0.0) / norm;
        VecScale(oi, scalar);
        PetscHelper::setRow(O_, oi, i);
    }
    MatAssemblyBegin(O_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(O_, MAT_FINAL_ASSEMBLY);
}



///////////////////////////////////////////////////////////////////////////////
PetscFunctionalMaps::~PetscFunctionalMaps() {
    MatDestroy(&C_);
    MatDestroy(&O_);
    MatDestroy(&A_);
    MatDestroy(&B_);
    
    MatDestroy(&PhiTM1_);
    MatDestroy(&PhiTM2_);
    
    MatDestroy(&Phi1_);
    MatDestroy(&Phi2_);
}


///////////////////////////////////////////////////////////////////////////////
vtkSmartPointer<vtkDoubleArray> PetscFunctionalMaps::transferFunction(vtkSmartPointer<vtkDataArray> f) {
    if(f->GetNumberOfTuples() != shape2_->getPolyData()->GetNumberOfPoints()) {
        throw invalid_argument(string("Number of tuples in f and number of points of the shape do not coincide in ").append(__PRETTY_FUNCTION__));
    }
    
    if(f->GetNumberOfComponents() != 1) {
        throw invalid_argument(string("Number of components of f is not equal to 1 in ").append(__PRETTY_FUNCTION__));
    }
    
    Vec fv;
    VecCreateSeq(PETSC_COMM_SELF, f->GetNumberOfTuples(), &fv);
    PetscHelper::vtkDataArrayToPetscVec(f, fv);
    
    
    Vec a;
    MatGetVecs(PhiTM1_, NULL, &a);
    MatMult(PhiTM1_, fv, a);
    
    
    Vec b;
    MatGetVecs(C_, NULL, &b);
    MatMult(C_, a, b);
    
    Vec Tfv;
    MatGetVecs(Phi2_, NULL, &Tfv);
    
    MatMult(Phi2_, b, Tfv);
    vtkSmartPointer<vtkDoubleArray> Tf = PetscHelper::petscVecToVtkDoubleArray(Tfv);
    
    VecDestroy(&fv);
    VecDestroy(&Tfv);
    VecDestroy(&a);
    VecDestroy(&b);
    
    return Tf;
}


///////////////////////////////////////////////////////////////////////////////
void PetscFunctionalMaps::setupPhiTM(Shape* shape, PetscLaplaceBeltramiOperator* laplacian, Mat* Phi, Mat *PhiTM) {
    PetscInt numberOfPoints = shape->getPolyData()->GetNumberOfPoints();
    
    MatCreateSeqDense(MPI_COMM_SELF, numberOfPoints, numberOfEigenfunctions_, NULL, Phi);
    laplacian->getEigenfunctionMatrix(Phi, numberOfEigenfunctions_);
    
    Mat PhiT;
    MatTranspose(*Phi, MAT_INITIAL_MATRIX, &PhiT);
    
    //get reference to mass matrix
    Mat* M = laplacian->getMassMatrix();
    MatMatMult(PhiT, *M, MAT_INITIAL_MATRIX, PETSC_DEFAULT, PhiTM);
    
    MatDestroy(&PhiT);
}

