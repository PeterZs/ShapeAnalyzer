//
//  CoordinateColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

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

class CoordinateColoring : public Coloring {
public:
    CoordinateColoring(Shape* shape);
    ~CoordinateColoring() {}
    
    virtual void color();
    
    vtkSmartPointer<vtkUnsignedCharArray> getColors() {
        return colors_;
    }
private:
    void calculateColors();
    
    vtkSmartPointer<vtkUnsignedCharArray> colors_;
};

#endif
