//
//  PointSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__PointSignature__
#define __ShapeAnalyzer__PointSignature__


#include <slepceps.h>

#include "Signature.h"
#include "../attributes/ScalarPointAttribute.h"


class PointSignature : public Signature {
public:
    virtual ~PointSignature();
    
    virtual void initialize(Shape* shape, int dimension);
    
    void getComponent(int i, Vec* component);
    
    void getComponent(int i, ScalarPointAttribute& component);
    
protected:
    Mat signature_;
    PointSignature() {}
};

#endif /* defined(__ShapeAnalyzer__PointSignature__) */
