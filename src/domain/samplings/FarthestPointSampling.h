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

class FarthestPointSampling : public MetricSampling {
public:
    virtual ~FarthestPointSampling() {
    }
    
    
    void setSource(vtkIdType source) {
        source_ = source;
    }
    
    void setNumberOfPoints(vtkIdType numberOfPoints) {
        numberOfPoints_ = numberOfPoints;
    }
    
    static Sampling* create() {
        return new FarthestPointSampling();
    }
    
    
    static string getIdentifier() {
        return "fps";
    }
    
    
    virtual vtkSmartPointer<vtkIdList> getPoints();
private:
    FarthestPointSampling() {
    }
    vtkIdType source_;
    vtkIdType numberOfPoints_;
};

#endif /* defined(__ShapeAnalyzer__FarthestPointSampling__) */
