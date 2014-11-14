///
/// \brief Assigns an attribute of class T to each face.
/// \details For the primitive types double and int the classes ScalarFaceAtrribute
/// and DiscreteFaceAttribute exist, they should be used instead of this class.
/// \note Not finished. Maybe add a function to fill with values without petsc stuff?
///
/// \author Emanuel Laude and Zorah Lähner
///

#ifndef ShapeAnalyzer_ComplexFaceAttribute_h
#define ShapeAnalyzer_ComplexFaceAttribute_h

#include "../Shape.h"

namespace attribute {

template<class T>
class ComplexFaceAttribute {
    
public:
    /// Deletes the array storing all attributes. If you are storing pointers, the attribute
    /// objects will not be deleted.
    ~ComplexFaceAttribute() {
        delete [] values_;
    }
    
    /// \brief Basic constructor.
    /// \details Constructs an array of T objects with the size of the number
    /// of faces of the shape.
    ComplexFaceAttribute(Shape* shape) {
        shape_ = shape;
        values_ = new T[shape->getPolyData()->GetNumberOfCells()];
    }
        
    /// \brief Returns a pointer to the attribute array.
    /// @return The array has the size of the number of face the shape has and will be deleted
    /// when the ComplexFaceAttribute is deleted.
    T* getValues() { return values_; }
    /// Returns the shape this attributes describe.
    Shape* getShape() { return shape_; }
    
private:
    /// Shape the attributes describe.
    Shape* shape_;
    /// Array of attributes. The size of the array is equal to the number of faces of the shape.
    /// The order of the attributes should correspond the order of faces in the the vtkPolData
    /// structure of the shape.
    T* values_;
};
    
    
}


#endif
