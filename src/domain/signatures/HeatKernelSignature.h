//
//  HeatKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__HeatKernelSignature__
#define __ShapeAnalyzer__HeatKernelSignature__

#include "Signature.h"

class HeatKernelSignature : public Signature {
public:
    
    virtual void initialize(Shape* shape, LaplaceBeltramiOperator* laplacian, int dimension);
    
    static Signature* create() {
        return new HeatKernelSignature();
    }
    
    static string getIdentifier() {
        return "hks";
    }
    
private:
    HeatKernelSignature() {}
};

#endif /* defined(__ShapeAnalyzer__HeatKernelSignature__) */
