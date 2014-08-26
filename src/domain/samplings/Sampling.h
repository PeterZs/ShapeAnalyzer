//
//  ShapeSampling.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#ifndef __ShapeAnalyzer__ShapeSampling__
#define __ShapeAnalyzer__ShapeSampling__

#include <iostream>

#include <vtkIdList.h>
#include <vtkSmartPointer.h>

#include "../Shape.h"

using namespace std;

class Sampling {
    
public:
    virtual ~Sampling() {}
    
    virtual void initialize(Shape* shape, vtkIdType numberOfPoints);
    
    virtual vtkSmartPointer<vtkIdList> getPoints() = 0;
    
    Shape*  getShape()      { return shape_; }
    
protected:
    Sampling() {}
    

    Shape* shape_;
    vtkIdType numberOfPoints_;
};

#endif /* defined(__ShapeAnalyzer__ShapeSampling__) */
