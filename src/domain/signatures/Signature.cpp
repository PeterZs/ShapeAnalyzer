//
//  Signature.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#include "Signature.h"


void Signature::initialize(Shape* shape, LaplaceBeltramiOperator* laplacian, int dimension) {
    shape_ = shape;
    laplacian_ = laplacian;
    dimension_ = dimension;
    MatCreateSeqDense(MPI_COMM_SELF, dimension_, shape_->getPolyData()->GetNumberOfPoints(), NULL, &signature_);
}

Signature::~Signature() {
    MatDestroy(&signature_);
}

void Signature::getComponent(int i, ScalarPointAttribute &component) {
    const PetscScalar* row;
    MatGetRow(signature_, i, NULL, NULL, &row);
    ScalarPointAttribute::arrayToScalarPointAttribute(row, component);
}

void Signature::getComponent(int i, Vec *component) {
    PetscHelper::getRow(*component, signature_, i);
}