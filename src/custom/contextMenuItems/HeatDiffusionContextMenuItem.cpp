//
//  HeatDiffusionContextMenuItem.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#include "HeatDiffusionContextMenuItem.h"


void HeatDiffusionContextMenuItem::onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
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
        
        vtkSmartPointer<vtkDoubleArray> u0 = vtkSmartPointer<vtkDoubleArray>::New();
        u0->SetNumberOfValues(shape->getPolyData()->GetNumberOfPoints());
        for(vtkIdType i = 0; i < shape->getPolyData()->GetNumberOfPoints(); i++) {
            if(i == source) {
                u0->SetValue(i, 1.0);
            } else {
                u0->SetValue(i, 0.0);
            }
        }
        PetscFEMLaplaceBeltramiOperator laplacian(shape, 100);
        
        PetscHeatDiffusion heatDiffusion(shape, &laplacian, u0);
        vtkSmartPointer<vtkDoubleArray> ut = heatDiffusion.getHeat(t);

        shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
        coloring->type = Shape::Coloring::Type::PointScalar;
        coloring->values = ut;
        shape->setColoring(coloring);
    }
}
