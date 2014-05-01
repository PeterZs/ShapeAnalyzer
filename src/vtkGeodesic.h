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
    
private:
    // converts a (0, 1)-double value into a r/g/b value
    int convertDoubleToR(double v);
    int convertDoubleToG(double v);
    int convertDoubleToB(double v);
    
};

#endif
