//
//  Segmentation.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#ifndef __ShapeAnalyzer__Segmentation__
#define __ShapeAnalyzer__Segmentation__

#include "vtkIdList.h"
#include "vtkSmartPointer.h"

#include "../Shape.h"



class Segmentation {
public:
    virtual ~Segmentation() {}
    
    virtual void initialize(Shape* shape);
    
    virtual vtkSmartPointer<vtkIdList> getSegmentation() = 0;
protected:
    Segmentation() {}
private:
    Shape* shape_;
};

#endif /* defined(__ShapeAnalyzer__Segmentation__) */
