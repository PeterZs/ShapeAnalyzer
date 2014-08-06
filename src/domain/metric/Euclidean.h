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
#include "../Shape.h"


#include <vtkPoints.h>
#include <vtkSmartPointer.h>

class Euclidean : public Metric {
private:
    Euclidean() {}
    
public:
    static Metric* create() {
        return new Euclidean();
    }
    
    virtual double getDistance(vtkIdType a, vtkIdType b);
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source);
    virtual vtkIdType getPointFurthestToAllSources(vtkSmartPointer<vtkIdList> sources);
};

#endif /* defined(__ShapeAnalyzer__Euclidean__) */
