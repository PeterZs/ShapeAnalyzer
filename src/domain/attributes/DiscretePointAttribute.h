#ifndef ShapeAnalyzer_DiscretePointAttribute_h
#define ShapeAnalyzer_DiscretePointAttribute_h

#include <vtkIntArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "../Shape.h"

namespace attribute {

///
/// \brief Assigns an int value to each vertex of the shape.
///
/// \author Emanuel Laude and Zorah Lähner
///
class DiscretePointAttribute {
    
public:
    /// Basic Constructor.
    DiscretePointAttribute(Shape* shape);
    /// Empty Destructor.
    ~DiscretePointAttribute() {
    };
    
    /// \brief Returns the vtkIntArray containing the attributes.
    /// \details The size of the returned array should be equal to the number of vertices of the shape.
    /// The order of the values normally corresponds to the one in the vtkPolyData structure of the shape.
    vtkSmartPointer<vtkIntArray> getValues() { return values_; }
    /// Returns the shape this attributes describe.
    Shape* getShape() { return shape_; }
    
private:
    /// Shape the attributes describe.
    Shape* shape_;
    /// Array of attributes. The size of the array is equal to the number of vertices of the shape.
    /// The order of the attributes correspondes the order of vertices in the the vtkPolData
    /// structure of the shape.
    vtkSmartPointer<vtkIntArray> values_;
};
    
}

#endif
