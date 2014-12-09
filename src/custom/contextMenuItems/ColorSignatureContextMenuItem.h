//
//  ColorSignatureContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorSignatureContextMenuItem__
#define __ShapeAnalyzer__ColorSignatureContextMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/PetscLaplaceBeltramiOperator.h"
#include "../../domain/signatures/PetscLaplaceBeltramiSignature.h"

#include "../Factory.h"

#include <qinputdialog.h>

#include <vtkDoubleArray.h>

using namespace std;

template<class T = PetscLaplaceBeltramiSignature>
class ColorSignatureContextMenuItem : public CustomContextMenuItem {
public:
    ColorSignatureContextMenuItem<T>(Shape* shape, ShapeAnalyzerInterface* shapeAnalyzer) : CustomContextMenuItem(shape, shapeAnalyzer) {}
    
    virtual void onClick(vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
        bool ok;
        int i = QInputDialog::getInt(
                                     parent,
                                     "Component",
                                     "Choose component.",
                                     0,
                                     0,
                                     99,
                                     1,
                                     &ok
                                     );
        
        if (ok) {
            PetscFEMLaplaceBeltramiOperator laplacian(shape_, 100);
            
            T s(shape_, 100, &laplacian);

            vtkSmartPointer<vtkDoubleArray> component = s.getComponent(i);

            shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
            coloring->type = Shape::Coloring::Type::PointScalar;
            coloring->values = component;
            shape_->setColoring(coloring);
        }
    }
};

#endif /* defined(__ShapeAnalyzer__ColorSignatureContextMenuItem__) */
