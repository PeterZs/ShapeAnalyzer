//
//  HeatDiffusion.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef __ShapeAnalyzer__HeatDiffusion__
#define __ShapeAnalyzer__HeatDiffusion__

#include <slepceps.h>

#include <vtkPolyData.h>

#include "Shape.h"
#include "FEMLaplaceBeltramiOperator.h"
#include "attributes/ScalarPointAttribute.h"
#include "PetscHelper.h"

class HeatDiffusion {
public:
    HeatDiffusion(Shape* shape, ScalarPointAttribute& initialCondition, int numberOfEigenfunctions);
    ~HeatDiffusion();
    void getHeat(ScalarPointAttribute& heat, double t);
private:
    
    Shape* shape_;
    PetscInt numberOfEigenfunctions_;

    Vec PhiTMu0_;
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusion__) */
