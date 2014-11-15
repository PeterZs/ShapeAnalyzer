//
//  HeatDiffusionCustomMenuItem.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#include "HeatDiffusionCustomMenuItem.h"


void HeatDiffusionCustomMenuItem::onClick(Shape* shape, QWidget* parent) {
    bool ok;
    double t = QInputDialog::getDouble(
                                       parent,
                                       "Heat diffusion",
                                       "Choose diffusion time.",
                                       0,
                                       0,
                                       2147483647,
                                       2,
                                       &ok
                                       );
    if (!ok) {
        return;
    }
    
    vtkIdType source = QInputDialog::getInt(
                                            parent,
                                            "Source vertex",
                                            "Choose ID of source vertex.",
                                            0,
                                            0,
                                            shape->getPolyData()->GetNumberOfPoints()-1,
                                            1,
                                            &ok
                                            );
    
    if (ok) {
        
        ScalarPointAttribute u0(shape);
        for(vtkIdType i = 0; i < shape->getPolyData()->GetNumberOfPoints(); i++) {
            if(i == source) {
                u0.getScalars()->SetValue(i, 1.0);
            } else {
                u0.getScalars()->SetValue(i, 0.0);
            }
        }
        LaplaceBeltramiOperator* laplacian = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian->initialize(shape, 100);
        
        HeatDiffusion heatDiffusion(shape, laplacian, u0);
        ScalarPointAttribute ut(shape);
        heatDiffusion.getHeat(ut, t);
        delete laplacian;
        coloring::ScalarPointColoring coloring(shape, ut);
        coloring.color();
    }
}
