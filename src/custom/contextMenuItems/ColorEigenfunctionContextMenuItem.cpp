
#include "ColorEigenfunctionContextMenuItem.h"

///////////////////////////////////////////////////////////////////////////////
void custom::contextMenuItems::ColorEigenfunctionContextMenuItem::onClick(vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
    bool ok;
    int i = QInputDialog::getInt(
                                 parent,
                                 "Eigenfunction",
                                 "Choose an eigenfunction.",
                                 0,
                                 0,
                                 std::min((vtkIdType) 99, shape_->getPolyData()->GetNumberOfPoints()),
                                 1,
                                 &ok
                                 );
    // show eigenfunction
    if (ok) {
        try {
            PetscFEMLaplaceBeltramiOperator laplacian(shape_, 100);
            vtkSmartPointer<vtkDoubleArray> eigenfunction = laplacian.getEigenfunction(i);
            
            shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
            coloring->type = Shape::Coloring::Type::PointScalar;
            coloring->values = eigenfunction;
            shape_->setColoring(coloring);
        } catch(LaplaceBeltramiError& e) {
            QMessageBox::warning(parent, "Exception", e.what());
        }
    }
}
