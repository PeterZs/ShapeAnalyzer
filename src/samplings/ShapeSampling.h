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

#include <Shape.h>

class ShapeSampling {
    
public:
    virtual ~ShapeSampling() = 0;
    
    vtkSmartPointer<vtkIdList> getSampling() {
        return idList_;
    }
    
private:
    // list of ids of the points in the sampling
    vtkSmartPointer<vtkIdList> idList_;
};

#endif /* defined(__ShapeAnalyzer__ShapeSampling__) */
