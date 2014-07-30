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

#include <vtkPolyData.h>

template<class T>
class ComplexPointAttribute {
    
public:
    ComplexPointAttribute(Shape* shape) {
        shape_ = shape;
        values = new T[shape->getPolyData()->GetNumberOfPoints()];
    }
    
    ~ComplexPointAttribute() {
    };
    
    // the size of the returned array must be equal to the number of vertices of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    T* getValues() { return values_ }
    Shape* getShape() { return shape_; }
    
private:
    Shape* shape_;
    T* values_;
};


#endif
