
#include "HeatDiffusionContextMenuItem.h"

///////////////////////////////////////////////////////////////////////////////
void custom::contextMenuItems::HeatDiffusionContextMenuItem::onClick(vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
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
                                            shape_->getPolyData()->GetNumberOfPoints()-1,
                                            1,
                                            &ok
                                            );
    
    if (ok) {
        try {
            vtkSmartPointer<vtkDoubleArray> u0 = vtkSmartPointer<vtkDoubleArray>::New();
            u0->SetNumberOfValues(shape_->getPolyData()->GetNumberOfPoints());
            for(vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfPoints(); i++) {
                if(i == source) {
                    u0->SetValue(i, 1.0);
                } else {
                    u0->SetValue(i, 0.0);
                }
            }
            shared_ptr<PetscLaplaceBeltramiOperator> laplacian = make_shared<PetscFEMLaplaceBeltramiOperator>(shape_, 100);
            
            PetscHeatDiffusion heatDiffusion(shape_, laplacian, u0);
            vtkSmartPointer<vtkDoubleArray> ut = heatDiffusion.getHeat(t);
            
            shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
            coloring->type = Shape::Coloring::Type::PointScalar;
            coloring->values = ut;
            shape_->setColoring(coloring);
        } catch(LaplaceBeltramiError& e) {
            QMessageBox::warning(parent, "Exception", e.what());
        }
    }
}
