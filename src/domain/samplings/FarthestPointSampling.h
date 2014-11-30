#ifndef __ShapeAnalyzer__FarthestPointSampling__
#define __ShapeAnalyzer__FarthestPointSampling__

#include "MetricSampling.h"

namespace sampling {
    /// \brief Concrete class that computes a point sampling
    class FarthestPointSampling : public MetricSampling {
    public:
        /// \brief Constructor
        /// @param Shape* The shape that is sampled.
        /// @param Metric* A refernce to the metric that is used for the computation of the sampled points.
        /// @param vtkIdType Source point ID.
        /// @param vtkIdType Number of samples that are computed.
        FarthestPointSampling(Shape* shape, Metric* metric, vtkIdType source, vtkIdType numberOfPoints) : MetricSampling(shape, metric), source_(source), numberOfPoints_(numberOfPoints) {
        }
        
        /// \brief Virtual destructor.
        virtual ~FarthestPointSampling() {
        }
        
        virtual vtkSmartPointer<vtkIdList> getPoints();
    private:
        vtkIdType source_;
        vtkIdType numberOfPoints_;
    };
    
}

#endif /* defined(__ShapeAnalyzer__FarthestPointSampling__) */
