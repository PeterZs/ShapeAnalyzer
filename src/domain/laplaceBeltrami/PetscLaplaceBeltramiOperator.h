//
//  PetscLaplaceBeltramiOperator.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.11.14.
//
//

#ifndef __ShapeAnalyzer__PetscLaplaceBeltramiOperator__
#define __ShapeAnalyzer__PetscLaplaceBeltramiOperator__

#include <slepceps.h>

#include "../PetscHelper.h"

#include "LaplaceBeltramiOperator.h"

namespace laplaceBeltrami {
    class PetscLaplaceBeltramiOperator : public LaplaceBeltramiOperator {
    public:
        PetscLaplaceBeltramiOperator(Shape* shape, int numberOfEigenfunctions) : LaplaceBeltramiOperator(shape, numberOfEigenfunctions) {}
        
        // Tell the compiler that both getEigenfunction methods (the one from the Base class and the one declared here) are used since they are different
        using LaplaceBeltramiOperator::getEigenfunction;
        
        virtual void getEigenfunction(PetscInt i, Vec* phi) = 0;
        
        virtual void getEigenpair(PetscInt i, Vec* phi, PetscScalar* lambda) = 0;
        
        //returns matrix Phi of dimension 'numberOfEigenfunctions x numberOfPoints' containing the eigenfunctions as columns.
        virtual void getEigenfunctionMatrix(Mat* Phi);
        
        //returns a reference to the mass matrix used internally. LaplaceOperator object itself is responsible for deleting the Mat object.
        virtual Mat* getMassMatrix() = 0;
    };
    
}
#endif /* defined(__ShapeAnalyzer__PetscLaplaceBeltramiOperator__) */
