//
//  DiscretePointAttribute.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef ShapeAnalyzer_DiscretePointAttribute_h
#define ShapeAnalyzer_DiscretePointAttribute_h

#include <vtkIntArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "../Shape.h"

class DiscretePointAttribute {
    
public:
    DiscretePointAttribute(Shape* shape);
    
    ~DiscretePointAttribute() {
    };
    
    // the size of the returned array must coincide with the number of vertices of the shape
    // the ordering of the values must coincide with the ordering of the vertices in vtkPolyData
    vtkSmartPointer<vtkIntArray> getValues() { return values_; }
    Shape* getShape() { return shape_; }
    
private:
    Shape* shape_;
    vtkSmartPointer<vtkIntArray> values_;
};

#endif
