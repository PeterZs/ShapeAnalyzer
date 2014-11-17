//
//  FarthestPointSampling.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef __ShapeAnalyzer__FarthestPointSampling__
#define __ShapeAnalyzer__FarthestPointSampling__

#include "MetricSampling.h"

namespace sampling {

class FarthestPointSampling : public MetricSampling {
public:
    FarthestPointSampling(Shape* shape, Metric* metric, vtkIdType source, vtkIdType numberOfPoints) : MetricSampling(shape, metric), source_(source), numberOfPoints_(numberOfPoints) {
    }
    
    virtual ~FarthestPointSampling() {
    }
    
    virtual vtkSmartPointer<vtkIdList> getPoints();
private:
    vtkIdType source_;
    vtkIdType numberOfPoints_;
};
    
}

#endif /* defined(__ShapeAnalyzer__FarthestPointSampling__) */
