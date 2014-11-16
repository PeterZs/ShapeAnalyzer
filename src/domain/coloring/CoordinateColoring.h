#ifndef ShapeAnalyzer_CoordinateColoring_h
#define ShapeAnalyzer_CoordinateColoring_h

#include "../Shape.h"

#include "Coloring.h"

#include <vtkDoubleArray.h>
#include <vtkMapper.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>

namespace coloring {
    ///
    /// \brief Colors the shape using the coordinates of the vertices as indicators for the rgb value.
    /// \details The entries are
    /// the coordinates of each vertex but normalized to values between 0 and 255.
    /// \author Emanuel Laude and Zorah Lähner
    ///
    class CoordinateColoring : public Coloring {
    public:
        /// \brief Constructor.
        /// \details Includes the calculation of the rgb value for each vertex. The entries are
        /// the coordinates of each vertex but normalized to values between 0 and 255.
        CoordinateColoring(Shape* shape);
        /// Empty destructor.
        ~CoordinateColoring() {}
        
        /// Applies the rgb values to the mapper and rerenders the scene.
        virtual void color();
        
        /// \brief Returns an array with the rgb values for each vertex.
        /// \details The array has 3 columns and as many columns as the shape has vertices.
        vtkSmartPointer<vtkUnsignedCharArray> getColors() {
            return colors_;
        }
    private:
        /// \brief Calculates the rgb color for each vertex.
        /// \details Calculates the min and max coordinate values on each axis by iterating
        /// over all vertices. Then assigns each vertex their x- y- and z- coordinate as
        /// r- g- and b-value normalized on the interval 0 to 255.
        void calculateColors();
        
        vtkSmartPointer<vtkUnsignedCharArray> colors_;
    };
}

#endif
