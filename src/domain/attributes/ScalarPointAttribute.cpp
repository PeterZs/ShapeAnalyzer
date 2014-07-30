//
//  ScalarPointAttribute.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "ScalarPointAttribute.h"


ScalarPointAttribute::ScalarPointAttribute(Shape* shape) {
    shape_ = shape;
    scalars_ = vtkSmartPointer<vtkDoubleArray>::New();
    scalars_->SetNumberOfValues(shape->getPolyData()->GetNumberOfPoints());
}