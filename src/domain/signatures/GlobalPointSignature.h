//
//  GlobalPointSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__GlobalPointSignature__
#define __ShapeAnalyzer__GlobalPointSignature__

#include "Signature.h"

class GlobalPointSignature : public Signature {
public:
    
    virtual void initialize(Shape* shape, LaplaceBeltramiOperator* laplacian, int dimension);
    
    static Signature* create() {
        return new GlobalPointSignature();
    }
    
    static string getIdentifier() {
        return "gps";
    }
    
private:
    GlobalPointSignature() {}
};

#endif /* defined(__ShapeAnalyzer__GlobalPointSignature__) */
