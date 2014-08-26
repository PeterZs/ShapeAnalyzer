//
//  Sampling.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "Sampling.h"

void Sampling::initialize(Shape *shape, vtkIdType numberOfPoints) {
    shape_ = shape;
    numberOfPoints_ = numberOfPoints;
}
