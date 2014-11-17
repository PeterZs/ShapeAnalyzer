//
//  ColorEigenfunctionContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorEigenfunctionContextMenuItem__
#define __ShapeAnalyzer__ColorEigenfunctionContextMenuItem__

#include <string>
#include <tuple>

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/FEMLaplaceBeltramiOperator.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

class ColorEigenfunctionContextMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<CustomContextMenuItem>(new ColorEigenfunctionContextMenuItem());
    }
    
private:
    ColorEigenfunctionContextMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__ColorEigenfunctionContextMenuItem__) */
