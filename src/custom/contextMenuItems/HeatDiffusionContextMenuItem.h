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

#include "../../domain/laplaceBeltrami/FEMLaplaceBeltramiOperator.h"
#include "../../domain/HeatDiffusion.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;
using namespace laplaceBeltrami;

class HeatDiffusionContextMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    

    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<HeatDiffusionContextMenuItem>(new HeatDiffusionContextMenuItem());
    }
    
private:
    HeatDiffusionContextMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusionContextMenuItem__) */
