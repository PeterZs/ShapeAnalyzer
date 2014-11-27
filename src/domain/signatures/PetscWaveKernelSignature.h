//
//  PetscWaveKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__PetscWaveKernelSignature__
#define __ShapeAnalyzer__PetscWaveKernelSignature__

#include "PetscLaplaceBeltramiSignature.h"
#include "../Shape.h"
#include "../laplaceBeltrami/PetscLaplaceBeltramiOperator.h"
#include "../../custom/Factory.h"

#include <vtkPolyData.h>

#include <cmath>

//Implementation adapted from MATLAB code taken from http://www.di.ens.fr/~aubry/wks.html

using namespace laplaceBeltrami;
using namespace signature;

namespace signature {

class PetscWaveKernelSignature : public PetscLaplaceBeltramiSignature {
public:    
    PetscWaveKernelSignature(Shape* shape, int dimension, PetscLaplaceBeltramiOperator* laplacian, double wksVariance = 6.0);

private:
    double wksVariance_;
};
    
}

#endif /* defined(__ShapeAnalyzer__PetscWaveKernelSignature__) */
