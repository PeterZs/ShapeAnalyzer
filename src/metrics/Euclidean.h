//
//  Euclidean.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#ifndef __ShapeAnalyzer__Euclidean__
#define __ShapeAnalyzer__Euclidean__

#include <iostream>

#include "Metric.h"

#include <vtkPoints.h>
#include <Shape.h>
#include <vtkSmartPointer.h>

class Euclidean : public Metric {
public:
    Euclidean(Shape* shape);
    
    virtual double getDistance(unsigned a, unsigned b);
    virtual vector<double> getAllDistances(unsigned source);
    
};

#endif /* defined(__ShapeAnalyzer__Euclidean__) */
