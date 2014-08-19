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
    
    vtkSmartPointer<vtkIdList> getPoints() {
        return points_;
    }
    
    Shape*  getShape()      { return shape_; }
    
protected:
    Sampling() {}
    

    Shape* shape_;
    vtkIdType numberOfPoints_;
    
    // list of ids of the points in the sampling
    vtkSmartPointer<vtkIdList> points_;
};

#endif /* defined(__ShapeAnalyzer__ShapeSampling__) */
