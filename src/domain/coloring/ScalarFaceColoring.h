//
//  ScalarFaceColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_ScalarFaceColoring_h
#define ShapeAnalyzer_ScalarFaceColoring_h

#include <vtkCellData.h>

#include "Coloring.h"
#include "../attributes/ScalarFaceAttribute.h"
#include "../Shape.h"

class ScalarFaceColoring : public Coloring {
public:
    ScalarFaceColoring(Shape* shape, ScalarFaceAttribute& attribute) : Coloring(shape), attribute_(attribute) {}
    
    virtual void color();
    
protected:
    ScalarFaceAttribute& attribute_;
};


#endif
