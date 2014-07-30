//
//  ScalarFaceAttribute.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef ShapeAnalyzer_ScalarFaceAttribute_h
#define ShapeAnalyzer_ScalarFaceAttribute_h

#include <vtkFloatArray.h>

#include "../Shape.h"

class ScalarFaceAttribute {
    
public:
    ~ScalarFaceAttribute() {};
    
    ScalarFaceAttribute(Shape* shape) {
        shape_ = shape;
        values = new T[shape->getPolyData()->getNumberOfPoints()];
    }

    
    // the size of the returned array must be equal to the number of faces of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    T* getValues() { return values_ }
    Shape* getShape() { return shape_; }
    
private:
    Shape* shape_;
    T* values_;
};

#endif
