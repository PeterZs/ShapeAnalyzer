//
//  DiscreteFaceAttribute.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 14.08.14.
//
//

#include "DiscreteFaceAttribute.h"

attribute::DiscreteFaceAttribute::DiscreteFaceAttribute(Shape* shape) {
    shape_ = shape;
    values_ = vtkSmartPointer<vtkIntArray>::New();
    values_->SetNumberOfValues(shape->getPolyData()->GetNumberOfCells());
}