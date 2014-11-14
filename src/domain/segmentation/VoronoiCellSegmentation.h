//
//  VoronoiCellSegmentation.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#ifndef __ShapeAnalyzer__VoronoiCellSegmentation__
#define __ShapeAnalyzer__VoronoiCellSegmentation__

#include "Segmentation.h"
#include "../metric/Metric.h"
#include "../samplings/Sampling.h"

using namespace metric;
using namespace sampling;

class VoronoiCellSegmentation : public Segmentation {
public:
    virtual ~VoronoiCellSegmentation() {}
    
    static string getIdentifier() {
        return "VoronoiCellSegmentation";
    }
    
    static Segmentation* create() {
        return new VoronoiCellSegmentation();
    }
    
    virtual vtkSmartPointer<vtkIdList> getSegmentation();
    
    void setMetric(Metric* metric) {
        metric_ = metric;
    }
    
    void setSampling(Sampling* sampling) {
        sampling_ = sampling;
    }
    
private:
    VoronoiCellSegmentation() {}
    
    Metric* metric_;
    Sampling* sampling_;
};

#endif /* defined(__ShapeAnalyzer__VoronoiCellSegmentation__) */
