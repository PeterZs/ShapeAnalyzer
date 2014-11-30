#ifndef __ShapeAnalyzer__PetscLaplaceBeltramiSignature__
#define __ShapeAnalyzer__PetscLaplaceBeltramiSignature__

#include "Signature.h"

#include "../laplaceBeltrami/PetscLaplaceBeltramiOperator.h"

using namespace laplaceBeltrami;

namespace signature {
    /// \brief Abstract class for Laplace-Beltrami based signatures that use the PETSC framework.
    class PetscLaplaceBeltramiSignature : public Signature {
    public:
        /// \brief Constructor.
        /// @param Shape* The shape on which the signature is computed.
        /// @param int Number of components that the signature should have.
        /// @param PetscLaplaceBeltramiOperator* A reference to an instance of a PETSC based Laplace-Beltrami operator
        PetscLaplaceBeltramiSignature(Shape *shape, int dimension, PetscLaplaceBeltramiOperator* laplacian);
        
        /// \brief Virtual destructor
        virtual ~PetscLaplaceBeltramiSignature();
        
        void getComponent(int i, Vec* component);
        virtual vtkSmartPointer<vtkDoubleArray> getComponent(int i);
    protected:
        Mat signature_;
        PetscLaplaceBeltramiOperator* laplacian_;
    };
    
}

#endif /* defined(__ShapeAnalyzer__PetscLaplaceBeltramiSignature__) */
