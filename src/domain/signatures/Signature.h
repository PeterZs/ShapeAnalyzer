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
#include "../attributes/ScalarPointAttribute.h"
#include "../LaplaceBeltramiOperator.h"

class Signature {
public:
    virtual ~Signature();
    
    virtual void initialize(Shape* shape, LaplaceBeltramiOperator* laplacian, int dimension);
    
    int getDimension() {
        return dimension_;
    }
    
    void getComponent(int i, Vec* component);
    
    void getComponent(int i, ScalarPointAttribute& component);
    
    
protected:
    Mat signature_;
    Shape* shape_;
    int dimension_;
    LaplaceBeltramiOperator* laplacian_;
    

    Signature() {}
};

#endif /* defined(__ShapeAnalyzer__Signature__) */
