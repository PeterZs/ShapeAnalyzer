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
#include "../../domain/coloring/ScalarPointColoring.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

template<class T>
class ColorSignatureCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, QWidget* parent) {
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
            LaplaceBeltramiOperator* laplacian = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
            laplacian->initialize(shape, 100);
            
            LaplaceBeltramiSignature* s = Factory<LaplaceBeltramiSignature>::getInstance()->create(T::getIdentifier());
            s->setLaplacian(laplacian);
            s->initialize(shape, 100);
            
            ScalarPointAttribute component(shape);
            s->getComponent(i, component);
            delete s;
            delete laplacian;
            ScalarPointColoring coloring(shape, component);
            coloring.color();
        }
    }
    
    static CustomContextMenuItem* create() {
        return new ColorSignatureCustomMenuItem<T>();
    }
    
private:
    ColorSignatureCustomMenuItem<T>() {}
};

#endif /* defined(__ShapeAnalyzer__ColorSignatureCustomMenuItem__) */
