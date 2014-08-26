//
//  LaplaceBeltramiSignature.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#include "LaplaceBeltramiSignature.h"

LaplaceBeltramiSignature::~LaplaceBeltramiSignature() {
    MatDestroy(&signature_);
}

void LaplaceBeltramiSignature::initialize(Shape *shape, int dimension) {
    Signature::initialize(shape, dimension);
    MatCreateSeqDense(MPI_COMM_SELF, dimension_, shape_->getPolyData()->GetNumberOfPoints(), NULL, &signature_);
}

void LaplaceBeltramiSignature::getComponent(int i, ScalarPointAttribute &component) {
    const PetscScalar* row;
    MatGetRow(signature_, i, NULL, NULL, &row);
    ScalarPointAttribute::arrayToScalarPointAttribute(row, component);
}

void LaplaceBeltramiSignature::getComponent(int i, Vec *component) {
    PetscHelper::getRow(*component, signature_, i);
}