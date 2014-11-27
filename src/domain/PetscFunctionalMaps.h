//
//  PetscFunctionalMaps.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 02.08.14.
//
//

#ifndef __ShapeAnalyzer__PetscFunctionalMaps__
#define __ShapeAnalyzer__PetscFunctionalMaps__

#include "FunctionalMaps.h"

#include "laplaceBeltrami/PetscLaplaceBeltramiOperator.h"

#include "PetscHelper.h"

#include <slepceps.h>

using namespace laplaceBeltrami;

class PetscFunctionalMaps : public FunctionalMaps {
public:
    PetscFunctionalMaps(Shape* shape1, Shape* shape2, PetscLaplaceBeltramiOperator* laplacian1, PetscLaplaceBeltramiOperator* laplacian2, vector<vtkSmartPointer<vtkDoubleArray>>& c1, vector<vtkSmartPointer<vtkDoubleArray>>& c2, int numberOfEigenfunctions);
    
    vtkSmartPointer<vtkDoubleArray> transferFunction(vtkSmartPointer<vtkDoubleArray> f);
    
    ~PetscFunctionalMaps();
private:
    void setupPhiTM(Shape* shape, PetscLaplaceBeltramiOperator* laplacian, Mat* Phi, Mat *PhiTM);
    
    Mat C_;
    Mat AT_;
    Vec b_;
    
    Mat Phi1_;
    Mat Phi2_;
    Mat PhiTM1_;
    Mat PhiTM2_;
    
    PetscLaplaceBeltramiOperator* laplacian1_;
    PetscLaplaceBeltramiOperator* laplacian2_;

    KSP ksp_; //least squares solver
};


#endif /* defined(__ShapeAnalyzer__FunctionalMaps__) */

