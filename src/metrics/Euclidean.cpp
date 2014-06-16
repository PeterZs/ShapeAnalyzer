//
//  Euclidean.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "Euclidean.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
Euclidean::Euclidean(Shape* shape) {
    identifier_ = "Euclidean";
    shape_ = shape;
}


///////////////////////////////////////////////////////////////////////////////
// Public Functions from abstract class Metric
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
double Euclidean::getDistance(unsigned int a, unsigned int b) {
    double p1[3], p2[3];
    shape_->getPolyData()->GetPoints()->GetPoint(a, p1);
    shape_->getPolyData()->GetPoints()->GetPoint(b, p2);
    
    return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}


///////////////////////////////////////////////////////////////////////////////
vector<double> Euclidean::getAllDistances(unsigned source) {
    vector<double> distances;
    distances.resize(shape_->getPolyData()->GetPoints()->GetNumberOfPoints());
    
    for(int i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        distances[i] = getDistance(source, i);
    }
    
    return distances;
}

///////////////////////////////////////////////////////////////////////////////
unsigned Euclidean::getPointFurthestToAllSources(vtkSmartPointer<vtkIdList> sources) {
    double dist = 0;
    unsigned id;
    
    // iterate over all points on the shape
    for(int i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        double d = getDistance(i, sources->GetId(0));
        // iterate over all sources
        for (int j = 1; j < sources->GetNumberOfIds(); j++) {
            // test for minimum distance of sources
            if (getDistance(i, sources->GetId(j)) < d)
                d = getDistance(i, sources->GetId(j));
        }
        
        // test for maximum distance over all points
        if (d > dist)
            id = i;
    }
    
    return id;
}
