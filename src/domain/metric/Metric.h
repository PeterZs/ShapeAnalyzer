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

///
/// \brief Abstract class for the representation of a metric on the vertices of a shape.
/// \details Besides a pure pairwise distance function getDistance(vtkIdType, vtkIdType) this class provides functionality to obtion distance vectors getAllDistances(ScalarPointAttribute& distances, vtkIdType source) containing the distances from one source to all other vertices or a functions to create voronoi cells and farthest point samplings.
/// \author Emanuel Laude and Zorah Lähner
///

class Metric {
 
    
public:
    virtual ~Metric() {};
    
    virtual void initialize(Shape* shape) {
        shape_ = shape;
    }
    
    // returns distance between the points with id a and b
    virtual double getDistance(vtkIdType a, vtkIdType b) = 0;
    // returns the distances from point with id source to all other points
    // the result is ordered by point ids
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source) = 0;
    
    // returns the id of the point with the greatest distance to all points
    // in the source list
    virtual vtkIdType getFarthestPoint(vtkSmartPointer<vtkIdList> sources) = 0;
    
    // returns voronoi cells
    virtual vtkSmartPointer<vtkIdList> getVoronoiCells(vtkSmartPointer<vtkIdList> seeds) = 0;
    
    
    Shape*  getShape()          { return shape_; }
    
protected:
    Shape* shape_;
  
    Metric() {}
};

#endif /* defined(__ShapeAnalyzer__Metric__) */
