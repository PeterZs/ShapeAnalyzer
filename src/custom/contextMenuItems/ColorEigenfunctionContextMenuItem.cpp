//
//  ColorEigenfunctionContextMenuItem.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#include "ColorEigenfunctionContextMenuItem.h"


void ColorEigenfunctionContextMenuItem::onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
    bool ok;
    int i = QInputDialog::getInt(
                                 parent,
                                 "Eigenfunction",
                                 "Choose an eigenfunction.",
                                 0,
                                 0,
                                 std::min((vtkIdType) 99, shape->getPolyData()->GetNumberOfPoints()),
                                 1,
                                 &ok
                                 );
    // show eigenfunction
    if (ok) {
        ScalarPointAttribute eigenfunction(shape);
        
        FEMLaplaceBeltramiOperator laplacian(shape, 100);
        laplacian.getEigenfunction(i, eigenfunction);
        
        shape->colorPointsScalars(eigenfunction.getScalars());
    }
}
