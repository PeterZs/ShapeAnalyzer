//
//  vtkGeodesic.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.04.14.
//
//

#include "Shape.h"

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include "QVTKWidget.h"

#ifndef ShapeAnalyzer_vtkGeodesic_h
#define ShapeAnalyzer_vtkGeodesic_h

class vtkGeodesic {
    
public:
    vtkGeodesic() {};
    ~vtkGeodesic() {};
    
    // calculate geodesic from a random point to all other points and visualize them
    void visualizeGeodesic(Shape *shape, QVTKWidget *qvtkWidget);
    void calculateGeodesic_gpu(Shape *shape);
    
private:
    
    struct COLOUR {
        int r,g,b;
    };
    
    COLOUR GetColour(double v, double vmin, double vmax);
    
};

#endif
