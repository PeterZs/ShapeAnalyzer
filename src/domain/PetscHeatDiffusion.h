//
//  PetscHeatDiffusion.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef __ShapeAnalyzer__PetscHeatDiffusion__
#define __ShapeAnalyzer__PetscHeatDiffusion__

#include <slepceps.h>

#include <vtkPolyData.h>

#include "Shape.h"
#include "laplaceBeltrami/PetscFEMLaplaceBeltramiOperator.h"
#include "PetscHelper.h"

using namespace laplaceBeltrami;

class PetscHeatDiffusion {
public:
    PetscHeatDiffusion(Shape* shape, PetscLaplaceBeltramiOperator* laplacian, vtkSmartPointer<vtkDoubleArray> initialCondition);
    ~PetscHeatDiffusion();
    vtkSmartPointer<vtkDoubleArray> getHeat(double t);
private:
    
    Shape* shape_;
    PetscLaplaceBeltramiOperator* laplacian_;

    Vec PhiTMu0_;
};

#endif /* defined(__ShapeAnalyzer__PetscHeatDiffusion__) */
