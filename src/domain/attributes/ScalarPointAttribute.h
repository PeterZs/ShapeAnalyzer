///
/// \brief Assigns a double value to each vertex of the shape.
/// \details Additionally the class has static methods to transform
/// ScalarPointAttributes into Petsc-Vectors and back, as well as
/// create a ScalarPointAttribute out of a
/// \note Not finished. Maybe add a function to fill with values without petsc stuff?
///
/// \author Emanuel Laude and Zorah Lähner
///

#ifndef ShapeAnalyzer_ScalarPointAttribute_h
#define ShapeAnalyzer_ScalarPointAttribute_h

#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include <slepceps.h>

#include "../Shape.h"

class ScalarPointAttribute {
    
public:
    /// Basic Constructor.
    ScalarPointAttribute(Shape* shape);
    
    /// Empty Destructor.
    ~ScalarPointAttribute() {
    };
    
    /// \brief Returns the vtkDoubleArray containing the attributes.
    /// \details The size of the returned array should be equal to the number of vertices of the shape.
    /// The order of the values normally corresponds to the one in the vtkPolyData structure of the shape.
    vtkSmartPointer<vtkDoubleArray> getScalars() { return scalars_; }
    /// Returns the shape this attributes describe.
    Shape* getShape() { return shape_; }
    
    
    
    ///@{
    
    /// Converts a ScalarPointAttribute to a petsc vector.
    static void scalarPointAttributeToPetscVec(ScalarPointAttribute& attr, Vec& vec);
    
    /// Converts a petsc vector to a ScalarPointAttribute.
    static void petscVecToScalarPointAttribute(Vec& vec, ScalarPointAttribute& attr);
    
    
    /// Converts a petsc array to a ScalarPointAttribute.
    /// \note To Manu: Is that right?
    static void arrayToScalarPointAttribute(const PetscScalar* array, ScalarPointAttribute& attr);
    
    ///@}
    

private:
    /// Shape the attributes describe.
    Shape* shape_;
    /// Array of attributes. The size of the array is equal to the number of vertices of the shape.
    /// The order of the attributes correspondes the order of vertices in the the vtkPolData
    /// structure of the shape.
    vtkSmartPointer<vtkDoubleArray> scalars_;
};

#endif
