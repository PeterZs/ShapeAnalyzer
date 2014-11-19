//
//  Segmentation.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__Segmentation__
#define __ShapeAnalyzer__Segmentation__

#include "vtkIntArray.h"
#include "vtkSmartPointer.h"

#include "../Shape.h"

namespace segmentation {

class Segmentation {
public:
    Segmentation(Shape* shape) : shape_(shape) {}
    
    virtual ~Segmentation() {}
    
    virtual vtkSmartPointer<vtkIntArray> getSegments() = 0;
    
protected:
    Shape* shape_;
};
    
}

#endif /* defined(__ShapeAnalyzer__Segmentation__) */
