//
//  HeatKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__HeatKernelSignature__
#define __ShapeAnalyzer__HeatKernelSignature__

#include "PointSignature.h"

class HeatKernelSignature : public PointSignature {
public:
    
    virtual void initialize(Shape* shape, int dimension);
    
    static PointSignature* create() {
        return new HeatKernelSignature();
    }
    
private:
    HeatKernelSignature() {}
};

#endif /* defined(__ShapeAnalyzer__HeatKernelSignature__) */
