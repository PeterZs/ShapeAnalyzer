
#ifndef ShapeAnalyzer_ScalarFaceColoring_h
#define ShapeAnalyzer_ScalarFaceColoring_h

#include <vtkCellData.h>

#include "Coloring.h"
#include "../attributes/ScalarFaceAttribute.h"
#include "../Shape.h"

///
/// \brief Colors a shape given a ScalarFaceAttribute.
/// \details The range of the attributes is automatically retrieved and the color range adjusted.
///
class ScalarFaceColoring : public Coloring {
public:
    /// Constructor.
    /// @param Shape to be colored.
    /// @param Scalar values that determine the colors.
    ScalarFaceColoring(Shape* shape, ScalarFaceAttribute& attribute) : Coloring(shape), attribute_(attribute) {}
    
    /// Colors the shape.
    virtual void color();
    
protected:
    ScalarFaceAttribute& attribute_;
};


#endif
