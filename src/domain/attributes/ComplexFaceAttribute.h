//
//  FaceAttribute.h
//  ShapeAnalyzer
//
//  Assigns a double value to each face of the referenced shape.
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_ComplexFaceAttribute_h
#define ShapeAnalyzer_ComplexFaceAttribute_h

#include "../Shape.h"

template<class T>
class ComplexFaceAttribute {
    
public:
    ~ComplexFaceAttribute() {
        delete [] values_;
    }
    
    ComplexFaceAttribute(Shape* shape) {
        shape_ = shape;
        values_ = new T[shape->getPolyData()->GetNumberOfCells()];
    }
        
    // the size of the returned array must be equal to the number of faces of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    T* getValues() { return values_; }
    Shape* getShape() { return shape_; }
    
private:
    Shape* shape_;
    T* values_;
};


#endif
