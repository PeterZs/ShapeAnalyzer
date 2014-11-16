#ifndef __ShapeAnalyzer__ShapeSampling__
#define __ShapeAnalyzer__ShapeSampling__

#include <iostream>

#include <vtkIdList.h>
#include <vtkSmartPointer.h>

#include "../Shape.h"

using namespace std;

namespace sampling {

///
/// \brief Abstract class for sub sampling points on shapes.
///
class Sampling {
    
public:
    /// Virtual empty destructor.
    virtual ~Sampling() {}
    
    /// \brief Initializes the sampling with the corresponding Shape.
    virtual void initialize(Shape* shape) {
        shape_ = shape;
    }
    
    /// \brief Returns a list of the ids of the sampled points.
    virtual vtkSmartPointer<vtkIdList> getPoints() = 0;
    
    /// \brief Returns a pointer to the shape.
    Shape*  getShape()      { return shape_; }
    
protected:
    /// \brief Basic constructor.
    Sampling() {}

    Shape* shape_;
};
    
}

#endif /* defined(__ShapeAnalyzer__ShapeSampling__) */
