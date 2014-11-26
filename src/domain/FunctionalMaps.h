//
//  FunctionalMaps.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 02.08.14.
//
//

#ifndef __ShapeAnalyzer__FunctionalMaps__
#define __ShapeAnalyzer__FunctionalMaps__

#include "Shape.h"
#include "PetscHelper.h"
#include "laplaceBeltrami/LaplaceBeltramiOperator.h"

#include <vtkDoubleArray.h>

#include <vector>

#include <slepceps.h>

using namespace laplaceBeltrami;

class FunctionalMaps {
public:
    FunctionalMaps(Shape* shape1, Shape* shape2, LaplaceBeltramiOperator* laplacian1, LaplaceBeltramiOperator* laplacian2, vector<vtkSmartPointer<vtkDoubleArray>>& c1, vector<vtkSmartPointer<vtkDoubleArray>>& c2, int numberOfEigenfunctions);
    
    vtkSmartPointer<vtkDoubleArray> transferFunction(vtkSmartPointer<vtkDoubleArray> f);
    
    ~FunctionalMaps();
private:
    void setupPhiTM(Shape* shape, LaplaceBeltramiOperator* laplacian, Mat* Phi, Mat *PhiTM);
    
    Mat C_;
    Mat AT_;
    Vec b_;
    
    Mat Phi1_;
    Mat Phi2_;
    Mat PhiTM1_;
    Mat PhiTM2_;
    
    
    Shape* shape1_;
    Shape* shape2_;
    LaplaceBeltramiOperator* laplacian1_;
    LaplaceBeltramiOperator* laplacian2_;
    vector<vtkSmartPointer<vtkDoubleArray>> c1_;
    vector<vtkSmartPointer<vtkDoubleArray>> c2_;
    int numberOfEigenfunctions_;
    PetscInt numberOfConstraints_;
    KSP ksp_; //least squares solver
};


#endif /* defined(__ShapeAnalyzer__FunctionalMaps__) */

