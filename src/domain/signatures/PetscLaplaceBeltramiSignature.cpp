#include "PetscLaplaceBeltramiSignature.h"

///////////////////////////////////////////////////////////////////////////////
signature::PetscLaplaceBeltramiSignature::PetscLaplaceBeltramiSignature(Shape *shape, int dimension, PetscLaplaceBeltramiOperator* laplacian) : Signature(shape, dimension), laplacian_(laplacian) {
    // argument check
    if (laplacian_ == nullptr) {
        throw invalid_argument(string("Null pointer input 'laplacian' in ").append(__PRETTY_FUNCTION__));
    }
    MatCreateSeqDense(MPI_COMM_SELF, dimension_, shape_->getPolyData()->GetNumberOfPoints(), NULL, &signature_);
}


///////////////////////////////////////////////////////////////////////////////
signature::PetscLaplaceBeltramiSignature::~PetscLaplaceBeltramiSignature() {
    MatDestroy(&signature_);
}


///////////////////////////////////////////////////////////////////////////////
vtkSmartPointer<vtkDoubleArray> signature::PetscLaplaceBeltramiSignature::getComponent(int i) {
    const PetscScalar* row;
    MatGetRow(signature_, i, NULL, NULL, &row);
    vtkSmartPointer<vtkDoubleArray> component = vtkSmartPointer<vtkDoubleArray>::New();
    component->SetNumberOfValues(shape_->getPolyData()->GetNumberOfPoints());
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfPoints(); i++) {
        component->SetValue(i, row[i]);
    }
    
    return component;
}


///////////////////////////////////////////////////////////////////////////////
void signature::PetscLaplaceBeltramiSignature::getComponent(int i, Vec *component) {
    PetscHelper::getRow(*component, signature_, i);
}