#ifndef ShapeAnalyzer_Coloring_h
#define ShapeAnalyzer_Coloring_h

#include "../Shape.h"

namespace coloring {
    
    ///
    /// \brief Abstract class for defining colors on shape surfaces.
    ///
    /// \author Emanuel Laude and Zorah Lähner
    ///
    class Coloring {
    public:
        /// Constructor.
        Coloring(Shape* shape) : shape_(shape) { }
        
        /// Function will initiate coloring of the shape.
        virtual void color() = 0;
        
    protected:
        Shape* shape_;
    };
    
}

#endif
