#ifndef ShapeAnalyzer_ScalarFaceAttribute_h
#define ShapeAnalyzer_ScalarFaceAttribute_h

#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "../Shape.h"

///
/// \brief Assigns a double value to each face of the shape.
/// \note Not finished. Maybe add a function to fill with values without petsc stuff?
///
/// \author Emanuel Laude and Zorah Lähner
///
class ScalarFaceAttribute {
    
public:
    /// Emtpty Destructor.
    ~ScalarFaceAttribute() {};
    /// Basic Constructor.
    ScalarFaceAttribute(Shape* shape);

    
    /// \brief Returns the vtkDoubleArray containing the attributes.
    /// \details The size of the returned array should be equal to the number face of the shape.
    /// The order of the values normally corresponds to the one in the vtkPolyData structure of the shape.
    vtkSmartPointer<vtkDoubleArray> getScalars() { return scalars_; }
    /// Returns the shape this attributes describe.
    Shape* getShape() { return shape_; }
    
private:
    /// Shape the attributes describe.
    Shape* shape_;
    /// Array of attributes. The size of the array is equal to the number of faces of the shape.
    /// The order of the attributes correspondes the order of faces in the the vtkPolData
    /// structure of the shape.
    vtkSmartPointer<vtkDoubleArray> scalars_;
};

#endif
