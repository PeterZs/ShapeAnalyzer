//
//  FEMLaplaceBeltramiOperator.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#include "FEMLaplaceBeltramiOperator.h"

FEMLaplaceBeltramiOperator::FEMLaplaceBeltramiOperator(Shape* shape, int numberOfEigenfunctions) : LaplaceBeltramiOperator(shape, numberOfEigenfunctions) {
}

//detroy all data structures
FEMLaplaceBeltramiOperator::~FEMLaplaceBeltramiOperator() {
    PetscErrorCode ierr;
    
    ierr = VecDestroy(&xr_);
    ierr = VecDestroy(&xi_);
    ierr = EPSDestroy(&eps_);
    ierr = MatDestroy(&C_);
    ierr = MatDestroy(&M_);
}


void FEMLaplaceBeltramiOperator::getCotanMatrix(Mat* C) {
    *C = C_;
}

void FEMLaplaceBeltramiOperator::getMassMatrix(Mat* M) {
    *M = M_;
}

void FEMLaplaceBeltramiOperator::initialize() {
    PetscErrorCode ierr;
    
    setupMatrices();
    
    MatGetVecs(C_, NULL, &xr_);
    MatGetVecs(C_, NULL, &xi_);
    
    
    // Create eigensolver context
    ierr = EPSCreate(PETSC_COMM_WORLD, &eps_);
    
    // Set operators. In this case, it is a generalized eigenvalue problem
    ierr = EPSSetOperators(eps_, C_, M_);
    
    
    // Set solver parameters at runtime
    // type: generalized hermitian
    EPSSetProblemType(eps_, EPS_GHEP);
    EPSSetTarget(eps_, -1e-5);
    EPSSetWhichEigenpairs(eps_, EPS_TARGET_MAGNITUDE);
    EPSSetType(eps_, EPSARPACK);
    vtkIdType numberOfPoints = shape_->getPolyData()->GetNumberOfPoints();
    
    EPSSetDimensions(eps_, min((vtkIdType) numberOfEigenfunctions_, numberOfPoints), PETSC_DECIDE, PETSC_DECIDE);
    ST st;
    EPSGetST(eps_, &st);
    STSetType(st, STSINVERT);
    
    EPSSetFromOptions(eps_);
    // Solve the eigensystem
    ierr = EPSSolve(eps_);
    
    EPSType        type;
    PetscInt nev;
    EPSGetType(eps_, &type);
    PetscPrintf(PETSC_COMM_WORLD," Solution method: %s\n\n",type);
    EPSGetDimensions(eps_,&nev,NULL,NULL);
    PetscPrintf(PETSC_COMM_WORLD," Number of requested eigenvalues: %D\n",nev);
    ierr = EPSPrintSolution(eps_, NULL);
}

//get number of non-zero elements per row (needed for efficient allocation of sparse matrices)
void FEMLaplaceBeltramiOperator::getNnz(PetscInt *nnz, vtkIdType numberOfPoints, vtkIdType numberOfFaces) {

    set<vtkIdType>* adjacencyList = new set<vtkIdType>[numberOfPoints];
    for(vtkIdType i = 0; i < numberOfFaces; i++) {
        vtkTriangle* face = reinterpret_cast<vtkTriangle*>(shape_->getPolyData()->GetCell(i));
        
        
        for(int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            int k = (i + 2) % 3;
            
            adjacencyList[face->GetPointId(i)].insert(face->GetPointId(j));
            adjacencyList[face->GetPointId(i)].insert(face->GetPointId(k));
        }
        
    }
    
    for(vtkIdType i = 0; i < numberOfPoints; i++) {
        nnz[i] = 1 + adjacencyList[i].size();
    }
    delete [] adjacencyList;
}

//get value of mass matrix at ij
PetscScalar FEMLaplaceBeltramiOperator::getMass(double *a, double *b, double *c) {
    return vtkTriangle::TriangleArea(a, b, c) / 12.0;
}

PetscScalar FEMLaplaceBeltramiOperator::getCotan(double *a, double *b, double *c) {
    double ca[3];
    ca[0] = a[0] - c[0];
    ca[1] = a[1] - c[1];
    ca[2] = a[2] - c[2];
    
    double norm = sqrt(ca[0]*ca[0] + ca[1]*ca[1] + ca[2]*ca[2]);
    ca[0] /= norm;
    ca[1] /= norm;
    ca[2] /= norm;
    
    double cb[3];
    cb[0] = b[0] - c[0];
    cb[1] = b[1] - c[1];
    cb[2] = b[2] - c[2];
    
    double adjacent = cb[0]*ca[0] + cb[1]*ca[1] + cb[2]*ca[2];
    
    double opposite = sqrt(pow(c[0]+adjacent*ca[0]-b[0], 2) + pow(c[1]+adjacent*ca[1]-b[1], 2) + pow(c[2]+adjacent*ca[2]-b[2], 2));
    
    double cotan = adjacent / opposite;
    return 0.5 * cotan;
}

//setup cotan and mass matrix for further computation (e.g. eigenfunctions)
void FEMLaplaceBeltramiOperator::setupMatrices() {    
    PetscErrorCode ierr;

    vtkIdType numberOfPoints = shape_->getPolyData()->GetNumberOfPoints();
    vtkIdType numberOfFaces = shape_->getPolyData()->GetNumberOfCells();
    
    //compute number of non-zero elements per row (nnz is indexed by row index)
    PetscInt* nnz = new PetscInt[numberOfPoints];
    getNnz(nnz, numberOfPoints, numberOfFaces);
    
    //allocate spares matrices
    ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, numberOfPoints, numberOfPoints, 0, nnz, &M_);
    //ierr = MatSetOption(M_, MAT_SPD, PETSC_TRUE);
    
    ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, numberOfPoints, numberOfPoints, 0, nnz, &C_);
    //ierr = MatSetOption(C_, MAT_SYMMETRIC, PETSC_TRUE);
    
    //fill matrices with their values
    //iterate over all faces
    for(vtkIdType x = 0; x < numberOfFaces; x++) {
        vtkTriangle* face = reinterpret_cast<vtkTriangle*>(shape_->getPolyData()->GetCell(x));
        
        //iterate over all edges ij of triangle ijk. k always lies on the opposite side of edge ij
        for(int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            int k = (i + 2) % 3;

            double a[3];
            face->GetPoints()->GetPoint(i, a);
            double b[3];
            face->GetPoints()->GetPoint(j, b);
            double c[3];
            face->GetPoints()->GetPoint(k, c);
            
            
            PetscScalar mass = getMass(a, b, c);
            ierr = MatSetValue(M_, face->GetPointId(i), face->GetPointId(j), mass, ADD_VALUES);
            ierr = MatSetValue(M_, face->GetPointId(j), face->GetPointId(i), mass, ADD_VALUES);
            ierr = MatSetValue(M_, face->GetPointId(i), face->GetPointId(i), mass, ADD_VALUES);
            ierr = MatSetValue(M_, face->GetPointId(j), face->GetPointId(j), mass, ADD_VALUES);
            
            PetscScalar cotan = getCotan(a, b, c);
            ierr = MatSetValue(C_, face->GetPointId(i), face->GetPointId(j), cotan, ADD_VALUES);
            ierr = MatSetValue(C_, face->GetPointId(j), face->GetPointId(i), cotan, ADD_VALUES);
            ierr = MatSetValue(C_, face->GetPointId(i), face->GetPointId(i), -cotan, ADD_VALUES);
            ierr = MatSetValue(C_, face->GetPointId(j), face->GetPointId(j), -cotan, ADD_VALUES);
        }
    }
    
    //assemble matrices
    ierr = MatAssemblyBegin(M_, MAT_FINAL_ASSEMBLY);
    ierr = MatAssemblyEnd(M_, MAT_FINAL_ASSEMBLY);
    

    
    ierr = MatAssemblyBegin(C_, MAT_FINAL_ASSEMBLY);
    ierr = MatAssemblyEnd(C_, MAT_FINAL_ASSEMBLY);
    
    delete [] nnz;
}

//returns i-th eigenfunctions
void FEMLaplaceBeltramiOperator::getEigenfunction(PetscInt i, PetscScalar** phi) {
    PetscErrorCode ierr;
    ierr = EPSGetEigenvector(eps_, i, xr_, xi_);
    PetscInt size;
    VecGetSize(xr_, &size);
    VecGetArray(xr_, phi);
}

void FEMLaplaceBeltramiOperator::getEigenfunction(PetscInt i, Vec* phi) {
    PetscErrorCode ierr;
    ierr = EPSGetEigenvector(eps_, i, xr_, xi_);
    *phi = xr_;
}

//returns i-th eigenvalue
double FEMLaplaceBeltramiOperator::getEigenvalue(vtkIdType i) {
    PetscErrorCode ierr;
    PetscScalar k;
    ierr = EPSGetEigenvalue(eps_, i, &k, NULL);
    
    return k;
}

//return i-th eigenfunctions as ScalarPointAttribute
void FEMLaplaceBeltramiOperator::getEigenfunction(vtkIdType i, ScalarPointAttribute& phi) {

    PetscErrorCode ierr;
    ierr = EPSGetEigenvector(eps_, i, xr_, xi_);
    PetscInt size;
    VecGetSize(xr_, &size);
    PetscScalar* x;
    VecGetArray(xr_, &x);
    
    for(int j = 0; j < size; j++) {
        phi.getScalars()->SetValue(j, x[j]);
    }
}
