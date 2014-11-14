//
//  EuclideanMetric.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "EuclideanMetric.h"
#include <limits>


///////////////////////////////////////////////////////////////////////////////
// Public Functions from abstract class Metric
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
double metric::EuclideanMetric::getDistance(vtkIdType a, vtkIdType b) {
    double p1[3], p2[3];
    shape_->getPolyData()->GetPoints()->GetPoint(a, p1);
    shape_->getPolyData()->GetPoints()->GetPoint(b, p2);
    
    return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}


///////////////////////////////////////////////////////////////////////////////
void metric::EuclideanMetric::getAllDistances(ScalarPointAttribute& distances, vtkIdType source) {
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        distances.getScalars()->SetValue(i, getDistance(source, i));
    }
}

///////////////////////////////////////////////////////////////////////////////
vtkIdType metric::EuclideanMetric::getFarthestPoint(vtkSmartPointer<vtkIdList> sources) {
    double maxDist = 0;
    vtkIdType id;
    
    // iterate over all points on the shape
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        double dist = std::numeric_limits<double>::infinity();
        // iterate over all sources
        for (vtkIdType j = 0; j < sources->GetNumberOfIds(); j++) {
            // test for minimum distance of sources
            double d = getDistance(i, sources->GetId(j));
            if (d < dist)
                dist = d;
        }
        
        // test for maximum distance over all points
        if (dist > maxDist) {
            maxDist = dist;
            id = i;
        }
    }
    
    return id;
}

vtkSmartPointer<vtkIdList> metric::EuclideanMetric::getVoronoiCells(vtkSmartPointer<vtkIdList> seeds) {
    vtkSmartPointer<vtkIdList> voronoiCells = vtkSmartPointer<vtkIdList>::New();
    voronoiCells->SetNumberOfIds(shape_->getPolyData()->GetPoints()->GetNumberOfPoints());
    

    for(vtkIdType i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        double minDist = std::numeric_limits<double>::infinity();
        for(vtkIdType j = 0; j < seeds->GetNumberOfIds(); j++) {
            double dist = getDistance(seeds->GetId(j), i);
            if(dist < minDist) {
                minDist = dist;
                voronoiCells->SetId(i, j);
            }
        }
            
    }
    
    return voronoiCells;
}
