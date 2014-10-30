///
/// \brief Abstract class for samplings that are based on a metric.
///

#ifndef __ShapeAnalyzer__MetricSampling__
#define __ShapeAnalyzer__MetricSampling__

#include "Sampling.h"
#include "../metric/Metric.h"

class MetricSampling : public Sampling {
public:
    /// \brief Sets the metric 
    void setMetric(Metric* metric);

protected:
    Metric* metric_;
};

#endif /* defined(__ShapeAnalyzer__MetricSampling__) */
