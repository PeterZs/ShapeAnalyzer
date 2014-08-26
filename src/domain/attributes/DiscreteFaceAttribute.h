//
//  DiscreteFaceAttribute.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#ifndef ShapeAnalyzer_DiscreteFaceAttribute_h
#define ShapeAnalyzer_DiscreteFaceAttribute_h

#include <vtkIntArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "../Shape.h"

class DiscreteFaceAttribute {
    
public:
    ~DiscreteFaceAttribute() {};
    
    DiscreteFaceAttribute(Shape* shape);

    
    // the size of the returned array must be equal to the number of faces of the shape
    // the order of the values must correspond to the one in the vtkPolyData structure of the shape
    vtkSmartPointer<vtkIntArray> getValues() { return values_; }
    Shape* getShape() { return shape_; }
    
private:
    Shape* shape_;
    vtkSmartPointer<vtkIntArray> values_;
};

#endif
