//
//  DiscreteFaceColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_DiscreteFaceColoring_h
#define ShapeAnalyzer_DiscreteFaceColoring_h

#include <vtkCellData.h>

#include "Coloring.h"
#include "../attributes/DiscreteFaceAttribute.h"
#include "../Shape.h"

class DiscreteFaceColoring : public Coloring {
public:
    DiscreteFaceColoring(Shape* shape, DiscreteFaceAttribute& attribute) : Coloring(shape), attribute_(attribute) {}
    
    virtual void color();
    
protected:
    DiscreteFaceAttribute& attribute_;
};


#endif
