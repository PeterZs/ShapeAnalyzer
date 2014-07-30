//
//  FEMLaplaceBeltramiOperator.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#include "FEMLaplaceBeltramiOperator.h"

FEMLaplaceBeltramiOperator::~FEMLaplaceBeltramiOperator() {

}

void FEMLaplaceBeltramiOperator::setupMatrices() {
    PetscErrorCode ierr;

    
    vtkIdType numberOfPoints = shape_->getPolyData()->GetNumberOfPoints();
    vtkIdType numberOfFaces = shape_->getPolyData()->GetNumberOfCells();

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
    
    PetscInt* nnz = new PetscInt[numberOfPoints];
    
    for(vtkIdType i = 0; i < numberOfPoints; i++) {
        nnz[i] = 1 + adjacencyList[i].size();
    }
    delete [] adjacencyList;
    
    
    cout << "Compute dim"<<endl;
    
    ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, numberOfPoints, numberOfPoints, 0, nnz, &M_);
    ierr = MatSetOption(M_, MAT_SPD, PETSC_TRUE);
    
    ierr = MatCreateSeqAIJ(PETSC_COMM_SELF, numberOfPoints, numberOfPoints, 0, nnz, &C_);
    ierr = MatSetOption(C_, MAT_SYMMETRIC, PETSC_TRUE);
    
    for(vtkIdType x = 0; x < numberOfFaces; x++) {
        vtkTriangle* face = reinterpret_cast<vtkTriangle*>(shape_->getPolyData()->GetCell(x));
        

        for(int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            int k = (i + 2) % 3;

            double a[3];
            face->GetPoints()->GetPoint(i, a);
            double b[3];
            face->GetPoints()->GetPoint(j, b);
            double c[3];
            face->GetPoints()->GetPoint(k, c);
            
            PetscScalar val = vtkTriangle::TriangleArea(a, b, c) / 12.0;
            ierr = MatSetValue(M_, face->GetPointId(i), face->GetPointId(j), val, ADD_VALUES);
            ierr = MatSetValue(M_, face->GetPointId(j), face->GetPointId(i), val, ADD_VALUES);
            ierr = MatSetValue(M_, face->GetPointId(i), face->GetPointId(i), val, ADD_VALUES);
            ierr = MatSetValue(M_, face->GetPointId(j), face->GetPointId(j), val, ADD_VALUES);
            
            
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
            
            //cout <<"adjacent()"<<adjacent <<endl;
            
            double opposite = sqrt(pow(c[0]+adjacent*ca[0]-b[0], 2) + pow(c[1]+adjacent*ca[1]-b[1], 2) + pow(c[2]+adjacent*ca[2]-b[2], 2));
            
            //cout <<"opposite()"<<opposite <<endl;
            
            double cotan = adjacent / opposite;
            val = 0.5 * cotan;
            ierr = MatSetValue(C_, face->GetPointId(i), face->GetPointId(j), val, ADD_VALUES);
            ierr = MatSetValue(C_, face->GetPointId(j), face->GetPointId(i), val, ADD_VALUES);
            ierr = MatSetValue(C_, face->GetPointId(i), face->GetPointId(i), -val, ADD_VALUES);
            ierr = MatSetValue(C_, face->GetPointId(j), face->GetPointId(j), -val, ADD_VALUES);
        }
    }
    
    cout << "Fill M"<<endl;
    
    cout << "Fill C"<<endl;
    
    ierr = MatAssemblyBegin(M_, MAT_FINAL_ASSEMBLY);
    ierr = MatAssemblyEnd(M_, MAT_FINAL_ASSEMBLY);
    
    cout << "Assemble M"<<endl;


    
    ierr = MatAssemblyBegin(C_, MAT_FINAL_ASSEMBLY);
    ierr = MatAssemblyEnd(C_, MAT_FINAL_ASSEMBLY);
    
    cout << "Assemble C"<<endl;
    
    delete [] nnz;
}

void FEMLaplaceBeltramiOperator::compute() {
    PetscErrorCode ierr;
    Vec xr, xi;
    SlepcInitializeNoArguments();

    setupMatrices();
    
    MatGetVecs(C_, NULL, &xr);
    MatGetVecs(C_, NULL, &xi);

    
    // Create eigensolver context
    ierr = EPSCreate(PETSC_COMM_WORLD, &eps_);
    
    // Set operators. In this case, it is a generalized eigenvalue problem
    ierr = EPSSetOperators(eps_, C_, M_);
    
    
    // Set solver parameters at runtime
    // type: generalized hermitian

    EPSSetProblemType(eps_, EPS_GHEP);
    EPSSetTarget(eps_, 0.0);
    EPSSetWhichEigenpairs(eps_, EPS_TARGET_MAGNITUDE);
    EPSSetDimensions(eps_, 100, PETSC_DECIDE, PETSC_DECIDE);
    ST st;
    EPSGetST(eps_, &st);
    STSetType(st, STSINVERT);
    
    EPSSetFromOptions(eps_);
    // Solve the eigensystem
    ierr = EPSSolve(eps_);


    EPSType        type;
    PetscInt nev;
    EPSGetType(eps_,&type);
    PetscPrintf(PETSC_COMM_WORLD," Solution method: %s\n\n",type);
    EPSGetDimensions(eps_,&nev,NULL,NULL);
    PetscPrintf(PETSC_COMM_WORLD," Number of requested eigenvalues: %D\n",nev);
    ierr = EPSPrintSolution(eps_, NULL);
    
    
    ierr = EPSGetEigenvector(eps_, 7, xr, xi);
    ierr = EPSSetFromOptions(eps_);
    
    
    vtkIdType numberOfPoints = shape_->getPolyData()->GetNumberOfPoints();
    eigenvector_ = new PetscScalar(numberOfPoints);
    
    VecGetArray(xr, &eigenvector_);
    

    double min = 10000000000;
    double max = -100000000000;
    for(int i = 0; i < numberOfPoints; i++) {
        cout << "val "<< eigenvector_[i]<<endl;
        if(eigenvector_[i]< min) {
            min = eigenvector_[i];
        }
        
        if(eigenvector_[i]> max) {
            max = eigenvector_[i];
        }
    }
    cout << "min" <<min<<endl;
    cout << "max" <<max<<endl;
    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
        lookupTable->SetRange(min, max);

        lookupTable->Build();
    
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < numberOfPoints; i++) {
        double dcolor[3];
        lookupTable->GetColor(eigenvector_[i], dcolor);
        
        cout << "color " << dcolor[0]<<" "<<
        dcolor[1]<<" "<<
        dcolor[2]<<" "<<endl;
        unsigned char color[3];
        for(unsigned int j = 0; j < 3; j++) {
            color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
        }
        
        colors->InsertNextTupleValue(color);
        
    }
    
    shape_->getPolyData()->GetPointData()->SetScalars(colors);
    
    shape_->getPolyData()->Modified();
    
    shape_->getPolyDataNormals()->Update();
    shape_->getPolyDataNormals()->Modified();
    
    shape_->getRenderer()->GetRenderWindow()->Render();
    
    ierr = EPSDestroy(&eps_);
    ierr = MatDestroy(&C_);
    ierr = MatDestroy(&M_);
    ierr = SlepcFinalize();
}