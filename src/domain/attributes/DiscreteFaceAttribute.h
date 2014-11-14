#ifndef ShapeAnalyzer_DiscreteFaceAttribute_h
#define ShapeAnalyzer_DiscreteFaceAttribute_h

#include <vtkIntArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "../Shape.h"

namespace attribute {

///
/// \brief Assigns an int value to each face of the shape.
/// \details This class is a wrapper class for vtkIntArray. It is needed to query the number of faces since this information cannot directly be queried from vtkIntArray. (Warning: The function getNumberOfComponents inherited from vtkAbstractArray does not return the number of elements in vtkIntArray)
///
/// \author Emanuel Laude and Zorah Lähner
///
class DiscreteFaceAttribute {
    
public:
    /// Empty Destructor.
    ~DiscreteFaceAttribute() {};
    /// Basic Constructor.
    DiscreteFaceAttribute(Shape* shape);

    
    /// \brief Returns the vtkIntArray containing the attributes.
    /// \details The size of the returned array should be equal to the number face of the shape.
    /// The order of the values normally corresponds to the one in the vtkPolyData structure of the shape.
    /// 
    vtkSmartPointer<vtkIntArray> getValues() { return values_; }
    /// Returns the shape this attributes describe.
    Shape* getShape() { return shape_; }
    
private:
    /// Shape the attributes describe.
    Shape* shape_;
    /// Array of attributes. The size of the array is equal to the number of faces of the shape.
    /// The order of the attributes correspondes the order of faces in the the vtkPolData
    /// structure of the shape.
    vtkSmartPointer<vtkIntArray> values_;
};
    
}

#endif
