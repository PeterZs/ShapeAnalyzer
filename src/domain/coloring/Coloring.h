//
//  Coloring.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef ShapeAnalyzer_Coloring_h
#define ShapeAnalyzer_Coloring_h

#include "../Shape.h"

class Coloring {
public:
    Coloring(Shape* shape) : shape_(shape) {
        
    }
    
    virtual void color() = 0;
protected:
    Shape* shape_;
};

#endif
