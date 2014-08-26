//
//  EuclideanMetric.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#ifndef __ShapeAnalyzer__EuclideanMetric__
#define __ShapeAnalyzer__EuclideanMetric__

#include <iostream>

#include "Metric.h"
#include "../Shape.h"


#include <vtkPoints.h>
#include <vtkSmartPointer.h>

class EuclideanMetric : public Metric {
private:
    EuclideanMetric() {}
    
public:
    static Metric* create() {
        return new EuclideanMetric();
    }
    
    static string getIdentifier() {
        return "euclidean";
    }
    
    virtual double getDistance(vtkIdType a, vtkIdType b);
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source);
    virtual vtkSmartPointer<vtkIdList> getVoronoiCells(vtkSmartPointer<vtkIdList> seeds);
    virtual vtkIdType getFarthestPoint(vtkSmartPointer<vtkIdList> sources);
};

#endif /* defined(__ShapeAnalyzer__EuclideanMetric__) */
