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

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/PetscFEMLaplaceBeltramiOperator.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;
using namespace laplaceBeltrami;

class ColorEigenfunctionContextMenuItem : public CustomContextMenuItem {
public:
    ColorEigenfunctionContextMenuItem(Shape* shape, ShapeAnalyzerInterface* shapeAnalyzer) : CustomContextMenuItem(shape, shapeAnalyzer) {}
    
    virtual void onClick(vtkIdType pointId, vtkIdType faceId, QWidget* parent);
};

#endif /* defined(__ShapeAnalyzer__ColorEigenfunctionContextMenuItem__) */
