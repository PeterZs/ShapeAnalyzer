//
//  ColorEigenfunctionCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorEigenfunctionCustomMenuItem__
#define __ShapeAnalyzer__ColorEigenfunctionCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/FEMLaplaceBeltramiOperator.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

class ColorEigenfunctionCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<CustomContextMenuItem>(new ColorEigenfunctionCustomMenuItem());
    }
    
private:
    ColorEigenfunctionCustomMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__ColorEigenfunctionCustomMenuItem__) */
