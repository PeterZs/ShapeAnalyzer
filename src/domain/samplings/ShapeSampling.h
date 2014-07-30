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

class ShapeSampling {
    
public:
    
    vtkSmartPointer<vtkIdList> getSampling() {
        return idList_;
    }
    
    string  getIdentifier() { return identifier_; }
    Shape*  getShape()      { return shape_; }
    
protected:
    // list of ids of the points in the sampling
    vtkSmartPointer<vtkIdList> idList_;
    
    string identifier_;
    Shape* shape_;
};

#endif /* defined(__ShapeAnalyzer__ShapeSampling__) */
