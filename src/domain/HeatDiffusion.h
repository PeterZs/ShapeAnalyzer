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

#include "Shape.h"
#include "FEMLaplaceBeltramiOperator.h"

class HeatDiffusion {
public:
    HeatDiffusion(Shape* shape, FEMLaplaceBeltramiOperator* laplacian);
private:
    Shape* shape_;
    Mat Phi_;
    Vec Lambda_;
    FEMLaplaceBeltramiOperator* laplacian_;
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusion__) */
