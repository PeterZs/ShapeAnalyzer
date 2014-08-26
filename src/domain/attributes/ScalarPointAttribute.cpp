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

void ScalarPointAttribute::scalarPointAttributeToPetscVec(ScalarPointAttribute &attr, Vec &vec) {
    PetscInt size = attr.getShape()->getPolyData()->GetNumberOfPoints();
    
    for(PetscInt i = 0; i < size; i++) {
        VecSetValue(vec, i, attr.getScalars()->GetValue(i), INSERT_VALUES);
    }
    
    VecAssemblyBegin(vec);
    VecAssemblyEnd(vec);
}

void ScalarPointAttribute::petscVecToScalarPointAttribute(Vec& vec, ScalarPointAttribute& attr) {
    PetscScalar* arr;
    VecGetArray(vec, &arr);
    for(PetscInt j = 0; j < attr.getShape()->getPolyData()->GetNumberOfPoints(); j++) {
        attr.getScalars()->SetValue(j, arr[j]);
    }
}

void ScalarPointAttribute::arrayToScalarPointAttribute(const PetscScalar *array, ScalarPointAttribute &attr) {
    for(PetscInt j = 0; j < attr.getShape()->getPolyData()->GetNumberOfPoints(); j++) {
        attr.getScalars()->SetValue(j, array[j]);
    }
}
