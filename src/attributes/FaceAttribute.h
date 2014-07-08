//
//  FaceAttribute.h
//  ShapeAnalyzer
//
//  Assigns a double value to each face of the referenced shape.
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_FaceAttribute_h
#define ShapeAnalyzer_FaceAttribute_h

#include "../Shape.h"

class FaceAttribute {
    
public:
    ~FaceAttribute() {};
    
    // the size of the returned array must be equal to the number of faces of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    virtual double[] getValues() = 0;
    virtual *Shape getShape() = { return shape_; }
    
protected:
    *Shape shape_;
    
};


#endif
