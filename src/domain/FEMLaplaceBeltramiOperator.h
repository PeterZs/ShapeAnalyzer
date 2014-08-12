//
//  FEMLaplaceBeltramiOperator.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#ifndef ShapeAnalyzer_FEMLaplaceBeltramiOperator_h
#define ShapeAnalyzer_FEMLaplaceBeltramiOperator_h

#include "LaplaceBeltramiOperator.h"

#include <slepceps.h>

#include <vtkTriangle.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <set>
#include <iostream>

using namespace std;

class FEMLaplaceBeltramiOperator : public LaplaceBeltramiOperator {
public:
    FEMLaplaceBeltramiOperator(vtkSmartPointer<vtkPolyData> polyData, int numberOfEigenfunctions);

    virtual double getEigenvalue(int i);
    
    virtual void getEigenfunction(PetscInt i, Vec* phi);
    
    virtual void getEigenpair(PetscInt i, Vec* phi, PetscScalar* lambda);
    
    //returns a reference to mass matrix. LaplaceOperator object itself is responsible for deleting the Mat object.
    virtual Mat* getMassMatrix();
    
    virtual ~FEMLaplaceBeltramiOperator();
    
private:

    void setupMatrices();
    
    inline void getNnz(PetscInt *nnz, vtkIdType numberOfPoints, vtkIdType numberOfFaces);
    inline PetscScalar getMass(double* a, double* b, double* c);
    inline PetscScalar getCotan(double* a, double* b, double* c);
    
    Mat C_;
    Mat M_;
    EPS eps_; // eigenproblem solver context
};

#endif
