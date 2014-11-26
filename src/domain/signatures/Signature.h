//
//  Signature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__Signature__
#define __ShapeAnalyzer__Signature__

#include "../Shape.h"
#include "../laplaceBeltrami/LaplaceBeltramiOperator.h"

namespace signature {

class Signature {
public:
    Signature(Shape* shape, int dimension) : shape_(shape), dimension_(dimension) {}
    
    virtual ~Signature() {}
    
    int getDimension() {
        return dimension_;
    }
    
    virtual vtkSmartPointer<vtkDoubleArray> getComponent(int i) = 0;
    
    
protected:
    Shape* shape_;
    int dimension_;
};
    
}

#endif /* defined(__ShapeAnalyzer__Signature__) */
