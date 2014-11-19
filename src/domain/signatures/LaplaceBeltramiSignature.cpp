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

void signature::LaplaceBeltramiSignature::getComponent(int i, ScalarPointAttribute &component) {
    const PetscScalar* row;
    MatGetRow(signature_, i, NULL, NULL, &row);
    ScalarPointAttribute::arrayToScalarPointAttribute(row, component);
}

void signature::LaplaceBeltramiSignature::getComponent(int i, Vec *component) {
    PetscHelper::getRow(*component, signature_, i);
}