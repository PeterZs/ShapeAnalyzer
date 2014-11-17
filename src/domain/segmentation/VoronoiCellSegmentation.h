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
    VoronoiCellSegmentation(Shape* shape, Metric* metric, Sampling* sampling) : Segmentation(shape), metric_(metric), sampling_(sampling) {
        
    }
    
    virtual ~VoronoiCellSegmentation() {}
    
    virtual vtkSmartPointer<vtkIdList> getSegmentation();
private:
    Metric* metric_;
    Sampling* sampling_;
};

#endif /* defined(__ShapeAnalyzer__VoronoiCellSegmentation__) */
