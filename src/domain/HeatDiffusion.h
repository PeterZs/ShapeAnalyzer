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
#include "laplaceBeltrami/FEMLaplaceBeltramiOperator.h"
#include "PetscHelper.h"

using namespace laplaceBeltrami;

class HeatDiffusion {
public:
    HeatDiffusion(Shape* shape, LaplaceBeltramiOperator* laplacian, vtkSmartPointer<vtkDoubleArray> initialCondition);
    ~HeatDiffusion();
    vtkSmartPointer<vtkDoubleArray> getHeat(double t);
private:
    
    Shape* shape_;
    LaplaceBeltramiOperator* laplacian_;

    Vec PhiTMu0_;
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusion__) */
