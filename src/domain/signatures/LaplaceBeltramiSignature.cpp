//
//  LaplaceBeltramiSignature.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#include "LaplaceBeltramiSignature.h"

signature::LaplaceBeltramiSignature::LaplaceBeltramiSignature(Shape *shape, int dimension, LaplaceBeltramiOperator* laplacian) : Signature(shape, dimension), laplacian_(laplacian) {
    MatCreateSeqDense(MPI_COMM_SELF, dimension_, shape_->getPolyData()->GetNumberOfPoints(), NULL, &signature_);
}

signature::LaplaceBeltramiSignature::~LaplaceBeltramiSignature() {
    MatDestroy(&signature_);
}

vtkSmartPointer<vtkDoubleArray> signature::LaplaceBeltramiSignature::getComponent(int i) {
    const PetscScalar* row;
    MatGetRow(signature_, i, NULL, NULL, &row);
    vtkSmartPointer<vtkDoubleArray> component = vtkSmartPointer<vtkDoubleArray>::New();
    component->SetNumberOfValues(shape_->getPolyData()->GetNumberOfPoints());
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfPoints(); i++) {
        component->SetValue(i, row[i]);
    }
    
    return component;
}

void signature::LaplaceBeltramiSignature::getComponent(int i, Vec *component) {
    PetscHelper::getRow(*component, signature_, i);
}