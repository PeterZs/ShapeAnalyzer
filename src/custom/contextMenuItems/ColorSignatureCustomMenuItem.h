//
//  ColorSignatureCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorSignatureCustomMenuItem__
#define __ShapeAnalyzer__ColorSignatureCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/LaplaceBeltramiOperator.h"
#include "../../domain/signatures/LaplaceBeltramiSignature.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

template<class T>
class ColorSignatureCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
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
            FEMLaplaceBeltramiOperator laplacian(shape, 100);
            
            T s(shape, 100, &laplacian);

            ScalarPointAttribute component(shape);
            s.getComponent(i, component);


            shape->colorPointsScalars(component.getScalars());
        }
    }
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<ColorSignatureCustomMenuItem<T>>(new ColorSignatureCustomMenuItem<T>());
    }
    
private:
    ColorSignatureCustomMenuItem<T>() {}
};

#endif /* defined(__ShapeAnalyzer__ColorSignatureCustomMenuItem__) */
