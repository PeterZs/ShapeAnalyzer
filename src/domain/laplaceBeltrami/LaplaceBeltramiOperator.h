//
//  LaplaceBeltramiOperator.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#ifndef ShapeAnalyzer_LaplaceBeltramiOperator_h
#define ShapeAnalyzer_LaplaceBeltramiOperator_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkDoubleArray.h>

#include <slepceps.h>

#include "../Shape.h"
#include "../PetscHelper.h"

namespace laplaceBeltrami {

class LaplaceBeltramiOperator {
public:
    LaplaceBeltramiOperator(Shape* shape, int numberOfEigenfunctions) : shape_(shape), numberOfEigenfunctions_(numberOfEigenfunctions) {
    }
    
    virtual ~LaplaceBeltramiOperator() {
    }
    
    virtual vtkSmartPointer<vtkDoubleArray> getEigenfunction(int i) = 0;
    
    virtual double getEigenvalue(int i) = 0;
    
    virtual void getEigenfunction(PetscInt i, Vec* phi) = 0;
    
    virtual void getEigenpair(PetscInt i, Vec* phi, PetscScalar* lambda) = 0;
    
    //returns matrix Phi of dimension 'numberOfEigenfunctions x numberOfPoints' containing the eigenfunctions as columns.
    virtual void getEigenfunctionMatrix(Mat* Phi);
    
    //returns a reference to the mass matrix used internally. LaplaceOperator object itself is responsible for deleting the Mat object.
    virtual Mat* getMassMatrix() = 0;
    
    int getNumberOfEigenfunctions() {
        return numberOfEigenfunctions_;
    }
protected:
    Shape* shape_; //the shape
    int numberOfEigenfunctions_;
};
    
}

#endif
