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

using namespace std;

class Metric {
    
public:
    virtual ~Metric() {};
    
    // returns distance between the points with id a and b
    virtual double          getDistance(unsigned a, unsigned b) = 0;
    // returns the distances from point with id source to all other points
    // the result is ordered by point ids
    virtual vector<double>  getAllDistances(unsigned source) = 0;
    
    string  getIdentifier() { return identifier_; }
    Shape*  getShape()      { return shape_; }
    
protected:
    string identifier_;
    Shape* shape_;
    
};

#endif /* defined(__ShapeAnalyzer__Metric__) */
