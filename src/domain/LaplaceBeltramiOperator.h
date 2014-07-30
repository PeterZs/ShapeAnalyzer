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

class LaplaceBeltramiOperator {
public:
    LaplaceBeltramiOperator(Shape* shape) : shape_(shape) {
        
    }
    
    virtual ~LaplaceBeltramiOperator() {};
    
    virtual void compute() = 0;
protected:
    Shape* shape_;
};

#endif
