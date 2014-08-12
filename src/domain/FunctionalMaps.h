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
#include "attributes/ScalarPointAttribute.h"
#include "PetscHelper.h"

#include <vector>

#include <slepceps.h>

class FunctionalMaps {
public:
    FunctionalMaps(Shape& shape1, Shape& shape2, vector<ScalarPointAttribute>& c1, vector<ScalarPointAttribute>& c2, int numberOfEigenfunctions);
    
    void transferFunction(ScalarPointAttribute& f1, ScalarPointAttribute& f2);
    
    ~FunctionalMaps();
private:
    void setupPhiTM(Shape& shape, Mat *Phi, Mat *PhiTM);
    
    Mat C_;
    Mat AT_;
    Vec B_;
    
    Mat Phi1_;
    Mat Phi2_;
    Mat PhiTM1_;
    Mat PhiTM2_;

    
    
    
    Shape& shape1_;
    Shape& shape2_;
    vector<ScalarPointAttribute> c1_;
    vector<ScalarPointAttribute> c2_;
    PetscInt numberOfEigenfunctions_;
    PetscInt numberOfConstraints_;
    KSP ksp_; //least squares solver
};


#endif /* defined(__ShapeAnalyzer__FunctionalMaps__) */
