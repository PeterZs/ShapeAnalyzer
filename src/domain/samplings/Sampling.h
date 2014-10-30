///
/// \brief Abstract class for sub sampling points on shapes.
///

#ifndef __ShapeAnalyzer__ShapeSampling__
#define __ShapeAnalyzer__ShapeSampling__

#include <iostream>

#include <vtkIdList.h>
#include <vtkSmartPointer.h>

#include "../Shape.h"

using namespace std;

class Sampling {
    
public:
    /// Virtual empty destructor.
    virtual ~Sampling() {}
    
    /// \brief Initializes the sampling.
    /// \note TODO Why does this numberOfPoints is here? Does not make sense for epsilon-sampling for example.
    virtual void initialize(Shape* shape, vtkIdType numberOfPoints) {
        shape_ = shape;
        numberOfPoints_ = numberOfPoints;
    }
    
    /// \brief Returns a list of the ids of the samples points.
    virtual vtkSmartPointer<vtkIdList> getPoints() = 0;
    
    /// \brief Returns a pointer to the shape.
    Shape*  getShape()      { return shape_; }
    
protected:
    /// \brief Basic constructor.
    Sampling() {}

    Shape* shape_;
    vtkIdType numberOfPoints_;
};

#endif /* defined(__ShapeAnalyzer__ShapeSampling__) */
