//
//  PetscLaplaceBeltramiSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#ifndef __ShapeAnalyzer__PetscLaplaceBeltramiSignature__
#define __ShapeAnalyzer__PetscLaplaceBeltramiSignature__

#include "Signature.h"

#include "../laplaceBeltrami/PetscLaplaceBeltramiOperator.h"

using namespace laplaceBeltrami;

namespace signature {
    
    class PetscLaplaceBeltramiSignature : public Signature {
    public:
        PetscLaplaceBeltramiSignature(Shape *shape, int dimension, PetscLaplaceBeltramiOperator* laplacian);
        
        virtual ~PetscLaplaceBeltramiSignature();
        
        void getComponent(int i, Vec* component);
        virtual vtkSmartPointer<vtkDoubleArray> getComponent(int i);
    protected:
        Mat signature_;
        PetscLaplaceBeltramiOperator* laplacian_;
    };
    
}

#endif /* defined(__ShapeAnalyzer__PetscLaplaceBeltramiSignature__) */
