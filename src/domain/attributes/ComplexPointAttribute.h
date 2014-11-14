#ifndef ShapeAnalyzer_PointAttribute_h
#define ShapeAnalyzer_PointAttribute_h

#include "../Shape.h"

#include <vtkPolyData.h>

namespace attribute {

///
/// \brief Assigns an attribute of class T to each vertex.
/// \details For the primitive types double and int the classes ScalarPointAtrribute
/// and DiscretePointAttribute exist, they should be used instead of this class.
/// \note Not finished. Maybe add a function to fill with values without petsc stuff?
///
/// \author Emanuel Laude and Zorah Lähner
///
template<class T>
class ComplexPointAttribute {
    
public:
    /// \brief Basic constructor.
    /// \details Constructs an array of T objects with the size of the number
    /// of vertices of the shape.
    ComplexPointAttribute(Shape* shape) {
        shape_ = shape;
        values_ = new T[shape->getPolyData()->GetNumberOfPoints()];
    }
    
    /// Deletes the array storing all attributes. If you are storing pointers, the attribute
    /// objects will not be deleted. 
    ~ComplexPointAttribute() {
        delete [] values_;
    }
    
    /// \brief Returns a pointer to the attribute array.
    /// @return The array has the size of the number of vertices the shape has and will be deleted
    /// when the ComplexPointAttribute is deleted.
    T* getValues() { return values_; }
    /// Returns the shape this attributes describe.
    Shape* getShape() { return shape_; }
    
private:
    /// Shape the attributes describe.
    Shape* shape_;
    /// Array of attributes. The size of the array is equal to the number of vertices of the shape.
    /// The order of the attributes should correspond the order of vertices in the the vtkPolData
    /// structure of the shape.
    T* values_;
};
    
}


#endif
