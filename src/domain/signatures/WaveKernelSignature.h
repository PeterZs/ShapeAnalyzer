//
//  WaveKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__WaveKernelSignature__
#define __ShapeAnalyzer__WaveKernelSignature__

#include "PointSignature.h"
#include "../Shape.h"

#include <vtkPolyData.h>

#include <cmath>

class WaveKernelSignature : public PointSignature {
public:    
    virtual void initialize(Shape* shape, int dimension);
    
    void setNumberOfEigenfunctions(int numberOfEigenfunctions) {
        numberOfEigenfunctions_ = numberOfEigenfunctions;
    }
    
    void setWKSVariance(double wksVariance) {
        wksVariance_ = wksVariance;
    }
    
    static PointSignature* create() {
        return new WaveKernelSignature();
    }
    
private:
    WaveKernelSignature() : numberOfEigenfunctions_(-1), wksVariance_(6.0) {}
    
    int numberOfEigenfunctions_;
    double wksVariance_;
};

#endif /* defined(__ShapeAnalyzer__WaveKernelSignature__) */
