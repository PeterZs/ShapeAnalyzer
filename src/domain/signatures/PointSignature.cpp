//
//  PointSignature.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#include "PointSignature.h"

void PointSignature::initialize(Shape* shape, int dimension) {
    Signature::initialize(shape, dimension);
    MatCreateSeqDense(MPI_COMM_SELF, dimension_, shape_->getPolyData()->GetNumberOfPoints(), NULL, &signature_);
}

PointSignature::~PointSignature() {
    MatDestroy(&signature_);
}

void PointSignature::getComponent(int i, ScalarPointAttribute &component) {
    const PetscScalar* row;
    MatGetRow(signature_, i, NULL, NULL, &row);
    ScalarPointAttribute::arrayToScalarPointAttribute(row, component);
}

void PointSignature::getComponent(int i, Vec *component) {
    PetscHelper::getRow(*component, signature_, i);
}