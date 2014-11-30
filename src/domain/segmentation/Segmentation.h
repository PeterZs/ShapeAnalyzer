#ifndef __ShapeAnalyzer__Segmentation__
#define __ShapeAnalyzer__Segmentation__

#include "vtkIntArray.h"
#include "vtkSmartPointer.h"

#include "../Shape.h"

/// \brief Namespace grouping all segmentation classes.
namespace segmentation {
    /// \brief Abstract class for computation of segments on a shape.
    class Segmentation {
    public:
        
        /// \brief Constructor.
        /// @param Shape* The shape on which a segmentation should be computed.
        Segmentation(Shape* shape) : shape_(shape) {}
        
        /// \brief Virtual destructor.
        virtual ~Segmentation() {}
        
        /// \brief Returns vertex-to-segment-ID mapping.
        virtual vtkSmartPointer<vtkIntArray> getSegments() = 0;
        
    protected:
        Shape* shape_;
    };
    
}

#endif /* defined(__ShapeAnalyzer__Segmentation__) */
