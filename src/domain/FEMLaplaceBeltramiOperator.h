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
    FEMLaplaceBeltramiOperator(Shape* shape) : LaplaceBeltramiOperator(shape) {
        
    }
    
    virtual void compute();
    
    
    virtual ~FEMLaplaceBeltramiOperator();
private:

    void setupMatrices();
    
    PetscScalar* eigenvector_;
    Mat C_;
    Mat M_;
    EPS eps_; // eigenproblem solver context
};

#endif
