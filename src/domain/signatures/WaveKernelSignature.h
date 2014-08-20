//
//  WaveKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__WaveKernelSignature__
#define __ShapeAnalyzer__WaveKernelSignature__

#include "Signature.h"
#include "../Shape.h"
#include "../LaplaceBeltramiOperator.h"
#include "../Factory.h"

#include <vtkPolyData.h>

#include <cmath>

class WaveKernelSignature : public Signature {
public:    
    virtual void initialize(Shape* shape, LaplaceBeltramiOperator* laplacian, int dimension);
    
    void setWKSVariance(double wksVariance) {
        wksVariance_ = wksVariance;
    }
    
    static Signature* create() {
        return new WaveKernelSignature();
    }
    
    static string getIdentifier() {
        return "wks";
    }
    
private:
    WaveKernelSignature() : wksVariance_(6.0) {}
    
    double wksVariance_;
};

#endif /* defined(__ShapeAnalyzer__WaveKernelSignature__) */
