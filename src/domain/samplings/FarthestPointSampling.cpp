//
//  FarthestPointSampling.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#include "FarthestPointSampling.h"

vtkSmartPointer<vtkIdList> FarthestPointSampling::getPoints() {
    vtkSmartPointer<vtkIdList> points = vtkSmartPointer<vtkIdList>::New();
    // calculate sampling
    if (source_ != -1)
        points->InsertNextId(source_);
    else
        points->InsertNextId(shape_->getRandomPoint());
    
    for (int i = 1; i < numberOfPoints_; i++) {
        // get point farthest away from all points in points_
        points->InsertNextId(metric_->getFarthestPoint(points));
    }

    return points;
}

