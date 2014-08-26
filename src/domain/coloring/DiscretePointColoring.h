//
//  DiscretePointColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_DiscretePointColoring_h
#define ShapeAnalyzer_DiscretePointColoring_h

#include <vtkPointData.h>

#include "Coloring.h"
#include "../attributes/DiscretePointAttribute.h"
#include "../Shape.h"

class DiscretePointColoring : public Coloring {
public:
    DiscretePointColoring(Shape* shape, DiscretePointAttribute& attribute) : Coloring(shape), attribute_(attribute) {}

    virtual void color();

protected:
    DiscretePointAttribute& attribute_;
};


#endif
