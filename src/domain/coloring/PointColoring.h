//
//  ShapePointColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_PointColoring_h
#define ShapeAnalyzer_PointColoring_h

#include <vtkPointData.h>

#include "Coloring.h"
#include "../attributes/ScalarPointAttribute.h"
#include "../Shape.h"

class PointColoring : public Coloring {
public:
    PointColoring(Shape* shape, ScalarPointAttribute* attribute) : Coloring(shape), attribute_(attribute) {}

    virtual void color();

protected:
    ScalarPointAttribute* attribute_;
};


#endif
