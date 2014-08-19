//
//  GlobalPointSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__GlobalPointSignature__
#define __ShapeAnalyzer__GlobalPointSignature__

#include "PointSignature.h"

class GlobalPointSignature : public PointSignature {
public:
    
    virtual void initialize(Shape* shape, int dimension);
    
    static PointSignature* create() {
        return new GlobalPointSignature();
    }
    
private:
    GlobalPointSignature() {}
};

#endif /* defined(__ShapeAnalyzer__GlobalPointSignature__) */
