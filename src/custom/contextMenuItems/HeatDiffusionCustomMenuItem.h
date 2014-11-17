//
//  HeatDiffusionCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__HeatDiffusionCustomMenuItem__
#define __ShapeAnalyzer__HeatDiffusionCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/FEMLaplaceBeltramiOperator.h"
#include "../../domain/HeatDiffusion.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

class HeatDiffusionCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    

    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<HeatDiffusionCustomMenuItem>(new HeatDiffusionCustomMenuItem());
    }
    
private:
    HeatDiffusionCustomMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__HeatDiffusionCustomMenuItem__) */
