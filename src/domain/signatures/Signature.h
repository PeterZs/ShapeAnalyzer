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

class Signature {
public:
    virtual void initialize(Shape* shape, int dimension);
    
    int getDimension() {
        return dimension_;
    }
    
protected:
    Shape* shape_;
    int dimension_;
    

    Signature() {}
};

#endif /* defined(__ShapeAnalyzer__Signature__) */
