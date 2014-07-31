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

class FEMLaplaceBeltramiOperator : public LaplaceBeltramiOperator {
public:
    FEMLaplaceBeltramiOperator(Shape* shape, int numberOfEigenfunctions);
    
    virtual void initialize();
    
    virtual void getEigenfunction(vtkIdType i, ScalarPointAttribute& eigenfunction);
    
    virtual double getEigenvalue(vtkIdType i);
    
    void getEigenfunction(PetscInt i, PetscScalar** eigenfunction);
    
    void getEigenvalues(PetscScalar** eigenfunction);
    
    Mat getMassMatrix();
    
    Mat getCotanMatrix();
    
    virtual ~FEMLaplaceBeltramiOperator();
    
private:

    void setupMatrices();
    
    inline void getNnz(PetscInt *nnz, vtkIdType numberOfPoints, vtkIdType numberOfFaces);
    inline PetscScalar getMass(double* a, double* b, double* c);
    inline PetscScalar getCotan(double* a, double* b, double* c);
    
    Mat C_;
    Mat M_;
    Vec xi_;
    Vec xr_;
    EPS eps_; // eigenproblem solver context
};

#endif
