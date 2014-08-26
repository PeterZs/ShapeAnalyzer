//
//  DiscretePointAttribute.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "DiscretePointAttribute.h"


DiscretePointAttribute::DiscretePointAttribute(Shape* shape) {
    shape_ = shape;
    values_ = vtkSmartPointer<vtkIntArray>::New();
    values_->SetNumberOfValues(shape->getPolyData()->GetNumberOfPoints());
}
