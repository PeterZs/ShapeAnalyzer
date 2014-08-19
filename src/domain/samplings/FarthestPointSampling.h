//
//  FarthestPointSampling.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef __ShapeAnalyzer__FarthestPointSampling__
#define __ShapeAnalyzer__FarthestPointSampling__

#include "Sampling.h"
#include "../Factory.h"

#include "../metric/Metric.h"

template<class METRIC>
class FarthestPointSampling : public Sampling {
public:
    virtual void initialize(Shape* shape, vtkIdType numberOfPoints);
    
    void setSource(vtkIdType source) {
        source_ = source;
    }
    
    static Sampling* create() {
        return new FarthestPointSampling<METRIC>();
    }
    
private:
    FarthestPointSampling() {}
    Metric* metric_;
    vtkIdType source_;

};

///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor
///////////////////////////////////////////////////////////////////////////////
template<class METRIC>
void FarthestPointSampling<METRIC>::initialize(Shape* shape, vtkIdType numberOfPoints) {
    Sampling::initialize(shape, numberOfPoints);
    
 
    // calculate sampling
    if (source_ != -1)
        points_->InsertNextId(source_);
    else
        points_->InsertNextId(shape_->getRandomPoint());
    
    for (int i = 1; i < numberOfPoints_; i++) {
        // get point farthest away from all points in points_
        points_->InsertNextId(metric_->getPointFarthestFromAllSources(points_));
    }
    
}

#endif /* defined(__ShapeAnalyzer__FarthestPointSampling__) */
