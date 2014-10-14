///
/// \brief Abstract class for all sorts of colorings.
/// \author Emanuel Laude and Zorah Lähner
///

#ifndef ShapeAnalyzer_Coloring_h
#define ShapeAnalyzer_Coloring_h

#include "../Shape.h"

class Coloring {
public:
    /// Constructor.
    Coloring(Shape* shape) : shape_(shape) { }
    
    /// Function will initiate coloring of the shape.
    virtual void color() = 0;
    
protected:
    Shape* shape_;
};

#endif
