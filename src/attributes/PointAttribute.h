//
//  PointAttribute.h
//  ShapeAnalyzer
//
//  Assings a double value to each vertex of the referenced shape.
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_PointAttribute_h
#define ShapeAnalyzer_PointAttribute_h

#include "../Shape.h"

class PointAttribute {
    
public:
    ~PointAttribute() {};
    
    // the size of the returned array must be equal to the number of vertices of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    virtual double[] getValues() = 0;
    virtual *Shape getShape() = { return shape_; }
    
protected:
    *Shape shape_;
    
};


#endif
