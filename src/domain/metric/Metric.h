//
//  Metric.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#ifndef __ShapeAnalyzer__Metric__
#define __ShapeAnalyzer__Metric__

#include <iostream>

#include "../Shape.h"
#include "../attributes/ScalarPointAttribute.h"

#include <vtkIdList.h>
#include <vtkSmartPointer.h>

using namespace std;

class Metric {
    
public:
    Metric(Shape* shape) : shape_(shape) {
        
    }
    virtual ~Metric() {};
    
    // returns distance between the points with id a and b
    virtual double getDistance(vtkIdType a, vtkIdType b) = 0;
    // returns the distances from point with id source to all other points
    // the result is ordered by point ids
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source = -1) = 0;
    
    // returns the id of the point with the greatest distance to all points
    // in the source list
    virtual vtkIdType getPointFurthestToAllSources(vtkSmartPointer<vtkIdList> sources) = 0;
    
    string  getIdentifier() { return identifier_; }
    Shape*  getShape()      { return shape_; }
    
protected:
    string identifier_;
    Shape* shape_;
    
};

#endif /* defined(__ShapeAnalyzer__Metric__) */
