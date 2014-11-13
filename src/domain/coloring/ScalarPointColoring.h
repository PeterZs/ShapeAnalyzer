
#ifndef ShapeAnalyzer_ScalarPointColoring_h
#define ShapeAnalyzer_ScalarPointColoring_h

#include <vtkPointData.h>

#include "Coloring.h"
#include "../attributes/ScalarPointAttribute.h"
#include "../Shape.h"

///
/// \brief Colors a shape given ScalarPointAttribute.
/// \details The range of the attributes is automatically retrieved and the color range adjusted.
///
class ScalarPointColoring : public Coloring {
public:
    /// Constructor.
    /// @param Shape to be colored.
    /// @param Scalar values that determine the colors.
    ScalarPointColoring(Shape* shape, ScalarPointAttribute& attribute) : Coloring(shape), attribute_(attribute) {}

    /// Colors the shape, the values are interpolated on the faces.
    virtual void color();

protected:
    ScalarPointAttribute& attribute_;
};


#endif
