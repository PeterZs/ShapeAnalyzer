//
//  HeatDiffusionContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__HeatDiffusionContextMenuItem__
#define __ShapeAnalyzer__HeatDiffusionContextMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/PetscFEMLaplaceBeltramiOperator.h"
#include "../../domain/PetscHeatDiffusion.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;
using namespace laplaceBeltrami;

class HeatDiffusionContextMenuItem : public CustomContextMenuItem {
public:
    HeatDiffusionContextMenuItem() {}
    
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusionContextMenuItem__) */
