#ifndef __ShapeAnalyzer__MetricSampling__
#define __ShapeAnalyzer__MetricSampling__

#include "Sampling.h"
#include "../metric/Metric.h"

using namespace metric;

namespace sampling {

///
/// \brief Abstract class for samplings that are based on a metric.
///
class MetricSampling : public Sampling {
public:
    /// \brief Basic constructor.
    MetricSampling(Shape* shape, Metric* metric) : Sampling(shape), metric_(metric) {}
    
protected:
    Metric* metric_;
};
    
}

#endif /* defined(__ShapeAnalyzer__MetricSampling__) */
