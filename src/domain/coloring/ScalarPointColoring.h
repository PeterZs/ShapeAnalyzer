//
//  ScalarPointColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_ScalarPointColoring_h
#define ShapeAnalyzer_ScalarPointColoring_h

#include <vtkPointData.h>

#include "Coloring.h"
#include "../attributes/ScalarPointAttribute.h"
#include "../Shape.h"

class ScalarPointColoring : public Coloring {
public:
    ScalarPointColoring(Shape* shape, ScalarPointAttribute& attribute) : Coloring(shape), attribute_(attribute) {}

    virtual void color();

protected:
    ScalarPointAttribute& attribute_;
};


#endif
