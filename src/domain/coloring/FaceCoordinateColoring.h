//
//  FaceCoordinateColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 06.08.14.
//
//

#ifndef ShapeAnalyzer_FaceCoordinateColoring_h
#define ShapeAnalyzer_FaceCoordinateColoring_h

#include "../Shape.h"

#include "CoordinateColoring.h"

#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkMapper.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>

class FaceCoordinateColoring : public CoordinateColoring {
public:
    FaceCoordinateColoring(Shape* shape) : CoordinateColoring(shape) {
        colors_ = vtkSmartPointer<vtkUnsignedCharArray>::New();
        calculateColors();
    }
    ~FaceCoordinateColoring() {}
    
    virtual void color() {
        shape_->getMapper()->SetScalarModeToUseCellData();
        shape_->getPolyData()->GetCellData()->SetScalars(colors_);
    }
    
    vtkSmartPointer<vtkUnsignedCharArray> getColors() {
        return colors_;
    }
private:
    void calculateColors() {
        vtkSmartPointer<vtkPoints> points = shape_->getPolyData()->GetPoints();
        
        // color values
        colors_->SetNumberOfComponents(3);
        colors_->SetNumberOfTuples(shape_->getPolyData()->GetNumberOfCells());
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
        for (vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfCells(); i++) {
            vtkSmartPointer<vtkIdList> pointIds = shape_->getPolyData()->GetCell(i)->GetPointIds();
            
            double point1[3], point2[3], point3[3], color[3];
            
            points->GetPoint(pointIds->GetId(0), point1);
            points->GetPoint(pointIds->GetId(1), point2);
            points->GetPoint(pointIds->GetId(2), point3);
            
            // interpolate points to get color
            color[0] = (point1[0] + point2[0] + point3[0]) / 3;
            color[1] = (point1[1] + point2[1] + point3[1]) / 3;
            color[2] = (point1[2] + point2[2] + point3[2]) / 3;
            
            // range colors from 0 to 255
            color[0] = (color[0] + xOffset) / xmax * 255;
            color[1] = (color[1] + yOffset) / ymax * 255;
            color[2] = (color[2] + zOffset) / zmax * 255;
            
            colors_->SetTuple(i, color);
        }
    }
    
    vtkSmartPointer<vtkUnsignedCharArray> colors_;
};

#endif
