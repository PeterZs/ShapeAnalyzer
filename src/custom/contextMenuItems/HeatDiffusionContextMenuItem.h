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
#include "../../domain/laplaceBeltrami/LaplaceBeltramiError.h"
#include "../../domain/PetscHeatDiffusion.h"

#include "../Factory.h"

#include <QInputDialog>
#include <QMessageBox>

using namespace std;
using namespace laplaceBeltrami;

class HeatDiffusionContextMenuItem : public CustomContextMenuItem {
public:
    HeatDiffusionContextMenuItem(Shape* shape, ShapeAnalyzerInterface* shapeAnalyzer) : CustomContextMenuItem(shape, shapeAnalyzer) {}
    
    virtual void onClick(vtkIdType pointId, vtkIdType faceId, QWidget* parent);
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusionContextMenuItem__) */
