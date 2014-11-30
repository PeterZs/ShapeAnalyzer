#ifndef __ShapeAnalyzer__VoronoiCellSegmentation__
#define __ShapeAnalyzer__VoronoiCellSegmentation__

#include "Segmentation.h"
#include "../metric/Metric.h"
#include "../samplings/Sampling.h"

using namespace metric;
using namespace sampling;

namespace segmentation {
    /// \brief Class for the computation of Voronoi cells on a shape.
    class VoronoiCellSegmentation : public Segmentation {
    public:
        /// \brief Constructor.
        /// @param Shape* The shape on which a segmentation should be computed.
        /// @param Metric* A refernence to a metric that is used for the computation of the Voronoi cells.
        /// @param Sampling* A sampling that is used to obtain the seeds for the segments
        VoronoiCellSegmentation(Shape* shape, Metric* metric, Sampling* sampling) : Segmentation(shape), metric_(metric), sampling_(sampling) {
            
        }
        
        /// \brief Virtual destructor
        virtual ~VoronoiCellSegmentation() {}
        
        virtual vtkSmartPointer<vtkIntArray> getSegments();
    private:
        Metric* metric_;
        Sampling* sampling_;
    };
    
}

#endif /* defined(__ShapeAnalyzer__VoronoiCellSegmentation__) */
