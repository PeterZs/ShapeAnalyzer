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
    LaplaceBeltramiOperator(Shape* shape);
    
    virtual ~LaplaceBeltramiOperator() {};
    
    virtual void initialize() = 0;
    
    virtual void getEigenfunction(vtkIdType i, ScalarPointAttribute& eigenfunction) = 0;
protected:
    Shape* shape_;
};

#endif
