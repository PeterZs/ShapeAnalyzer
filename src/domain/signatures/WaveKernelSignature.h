//
//  WaveKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__WaveKernelSignature__
#define __ShapeAnalyzer__WaveKernelSignature__

#include "LaplaceBeltramiSignature.h"
#include "../Shape.h"
#include "../laplaceBeltrami/LaplaceBeltramiOperator.h"
#include "../custom/Factory.h"

#include <vtkPolyData.h>

#include <cmath>

//Implementation adapted from MATLAB code taken from http://www.di.ens.fr/~aubry/wks.html

class WaveKernelSignature : public LaplaceBeltramiSignature {
public:    
    WaveKernelSignature(Shape* shape, int dimension, LaplaceBeltramiOperator* laplacian, double wksVariance = 6.0);

private:
    double wksVariance_;
};

#endif /* defined(__ShapeAnalyzer__WaveKernelSignature__) */
