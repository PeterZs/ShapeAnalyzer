//
//  LaplaceBeltramiOperator.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#ifndef ShapeAnalyzer_LaplaceBeltramiOperator_h
#define ShapeAnalyzer_LaplaceBeltramiOperator_h

#include "Shape.h"
#include "attributes/ScalarPointAttribute.h"

class LaplaceBeltramiOperator {
public:
    LaplaceBeltramiOperator(Shape* shape, int numberOfEigenfunctions);
    
    virtual ~LaplaceBeltramiOperator() {};
    
    virtual void initialize() = 0;
    
    virtual void getEigenfunction(vtkIdType i, ScalarPointAttribute& phi) = 0;
    
    virtual double getEigenvalue(vtkIdType i) = 0;
    
    int getNumberOfEigenfunctions() {
        return numberOfEigenfunctions_;
    }
protected:
    Shape* shape_;
    int numberOfEigenfunctions_;
};

#endif
