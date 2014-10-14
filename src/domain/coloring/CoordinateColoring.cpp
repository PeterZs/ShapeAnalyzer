//
//  CoordinateColoring.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 14.08.14.
//
//

#include "CoordinateColoring.h"

CoordinateColoring::CoordinateColoring(Shape* shape) : Coloring(shape) {
    colors_ = vtkSmartPointer<vtkUnsignedCharArray>::New();
    calculateColors();
}

///////////////////////////////////////////////////////////////////////////////
void CoordinateColoring::color() {
    shape_->getPolyData()->GetPointData()->SetScalars(colors_);
    shape_->getMapper()->SetScalarModeToUsePointData();
    shape_->getMapper()->SetColorModeToDefault();
    shape_->getMapper()->ScalarVisibilityOn();
}


///////////////////////////////////////////////////////////////////////////////
void CoordinateColoring::calculateColors() {
    vtkSmartPointer<vtkPoints> points = shape_->getPolyData()->GetPoints();
    
    // color values
    colors_->SetNumberOfComponents(3);
    colors_->SetNumberOfTuples(points->GetNumberOfPoints());
    colors_->SetName("Colors");
    
    // calculate range
    double xmin = 0, xmax = 0, ymin = 0, ymax = 0, zmin = 0, zmax = 0;
    
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
        double point[3];
        points->GetPoint(i, point);
        
        // determine range of points
        xmin = std::min(xmin, point[0]);
        ymin = std::min(ymin, point[1]);
        zmin = std::min(zmin, point[2]);
        xmax = std::max(xmax, point[0]);
        ymax = std::max(ymax, point[1]);
        zmax = std::max(zmax, point[2]);
    }
    
    double xOffset = 0, yOffset = 0, zOffset = 0;
    
    if (xmin < 0) {
        xOffset = xmin * (-1);
        xmin = xmin + xOffset;
        xmax = xmax + xOffset;
    }
    if (ymin < 0) {
        yOffset = ymin * (-1);
        ymin = ymin + yOffset;
        ymax = ymax + yOffset;
    }
    if (zmin < 0) {
        zOffset = zmin * (-1);
        zmin = zmin + zOffset;
        zmax = zmax + zOffset;
    }
    
    // calculate colors
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
        double point[3];
        points->GetPoint(i, point);
        
        point[0] = (point[0] + xOffset) / xmax * 255;
        point[1] = (point[1] + yOffset) / ymax * 255;
        point[2] = (point[2] + zOffset) / zmax * 255;
        
        colors_->SetTuple(i, point);
    }
}