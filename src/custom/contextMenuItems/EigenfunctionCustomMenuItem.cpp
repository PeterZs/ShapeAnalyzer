//
//  EigenfunctionCustomMenuItem.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#include "EigenfunctionCustomMenuItem.h"


void EigenfunctionCustomMenuItem::onClick(Shape* shape, QWidget* parent) {
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
        
        LaplaceBeltramiOperator* laplacian = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian->initialize(shape, 100);
        laplacian->getEigenfunction(i, eigenfunction);
        delete laplacian;
        
        coloring::ScalarPointColoring coloring(shape, eigenfunction);
        coloring.color();
    }
}
