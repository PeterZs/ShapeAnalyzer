//
//  HeatKernelSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__HeatKernelSignature__
#define __ShapeAnalyzer__HeatKernelSignature__

#include "LaplaceBeltramiSignature.h"

class HeatKernelSignature : public LaplaceBeltramiSignature {
public:
    
    virtual void initialize(Shape* shape, int dimension);
    
    static LaplaceBeltramiSignature* create() {
        return new HeatKernelSignature();
    }
    
    static string getIdentifier() {
        return "hks";
    }
    
private:
    HeatKernelSignature() {}
};

#endif /* defined(__ShapeAnalyzer__HeatKernelSignature__) */
