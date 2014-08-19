//
//  EuclideanMetric.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "EuclideanMetric.h"



///////////////////////////////////////////////////////////////////////////////
// Public Functions from abstract class Metric
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
double EuclideanMetric::getDistance(vtkIdType a, vtkIdType b) {
    double p1[3], p2[3];
    shape_->getPolyData()->GetPoints()->GetPoint(a, p1);
    shape_->getPolyData()->GetPoints()->GetPoint(b, p2);
    
    return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}


///////////////////////////////////////////////////////////////////////////////
void EuclideanMetric::getAllDistances(ScalarPointAttribute& distances, vtkIdType source) {
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        distances.getScalars()->SetValue(i, getDistance(source, i));
    }
}

///////////////////////////////////////////////////////////////////////////////
vtkIdType EuclideanMetric::getPointFarthestFromAllSources(vtkSmartPointer<vtkIdList> sources) {
    double dist = 0;
    vtkIdType id;
    
    // iterate over all points on the shape
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        double d = getDistance(i, sources->GetId(0));
        // iterate over all sources
        for (vtkIdType j = 1; j < sources->GetNumberOfIds(); j++) {
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
