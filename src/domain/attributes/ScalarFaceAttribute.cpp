//
//  ScalarFaceAttribute.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 14.08.14.
//
//

#include "ScalarFaceAttribute.h"

ScalarFaceAttribute::ScalarFaceAttribute(Shape* shape) {
    shape_ = shape;
    scalars_ = vtkSmartPointer<vtkDoubleArray>::New();
    scalars_->SetNumberOfValues(shape->getPolyData()->GetNumberOfCells());
}