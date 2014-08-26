//
//  GlobalPointSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__GlobalPointSignature__
#define __ShapeAnalyzer__GlobalPointSignature__

#include "LaplaceBeltramiSignature.h"

class GlobalPointSignature : public LaplaceBeltramiSignature {
public:
    
    virtual void initialize(Shape* shape, int dimension);
    
    static LaplaceBeltramiSignature* create() {
        return new GlobalPointSignature();
    }
    
    static string getIdentifier() {
        return "gps";
    }
    
private:
    GlobalPointSignature() {}
};

#endif /* defined(__ShapeAnalyzer__GlobalPointSignature__) */
