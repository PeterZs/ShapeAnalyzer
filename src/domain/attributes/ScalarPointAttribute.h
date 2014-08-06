//
//  ScalarPointAttribute.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef ShapeAnalyzer_ScalarPointAttribute_h
#define ShapeAnalyzer_ScalarPointAttribute_h

#include "../Shape.h"

#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

class Shape;

class ScalarPointAttribute {
    
public:
    ScalarPointAttribute(Shape* shape);
    
    ~ScalarPointAttribute() {
    };
    
    // the size of the returned array must be equal to the number of vertices of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    vtkSmartPointer<vtkDoubleArray> getScalars() { return scalars_; }
    Shape* getShape() { return shape_; }
    
private:
    Shape* shape_;
    vtkSmartPointer<vtkDoubleArray> scalars_;
};

#endif
