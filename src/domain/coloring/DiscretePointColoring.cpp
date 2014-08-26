//
//  DiscretePointColoring.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "DiscretePointColoring.h"

void DiscretePointColoring::color() {
    int range[2];
    attribute_.getValues()->GetValueRange(range);
    
    shape_->getPolyData()->GetPointData()->SetScalars(attribute_.getValues());
    shape_->getPolyData()->Modified();
    
    //shape_->getMapper()->SetLookupTable(lookupTable_);
    shape_->getMapper()->ScalarVisibilityOn();
    shape_->getMapper()->SetScalarModeToUsePointData();
    shape_->getMapper()->SetColorModeToMapScalars();
    shape_->getMapper()->SetScalarRange(range[0], range[1]);
    shape_->getMapper()->Modified();
    
    shape_->getPolyDataNormals()->Update();
    shape_->getPolyDataNormals()->Modified();
    
    shape_->getRenderer()->GetRenderWindow()->Render();
    
}

