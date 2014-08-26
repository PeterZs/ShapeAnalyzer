//
//  MetricSampling.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#ifndef __ShapeAnalyzer__MetricSampling__
#define __ShapeAnalyzer__MetricSampling__

#include "Sampling.h"
#include "../metric/Metric.h"

class MetricSampling : public Sampling {
public:
    void setMetric(Metric* metric);

protected:
    Metric* metric_;
};

#endif /* defined(__ShapeAnalyzer__MetricSampling__) */
