//
//  DiscreteFaceColoring.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 30.07.14.
//
//

#include "DiscreteFaceColoring.h"

///////////////////////////////////////////////////////////////////////////////
void coloring::DiscreteFaceColoring::color() {
    double range[2];
    attribute_.getValues()->GetRange(range);
    
    shape_->getPolyData()->GetCellData()->SetScalars(attribute_.getValues());
    shape_->getPolyData()->Modified();
    
    //shape_->getMapper()->SetLookupTable(lookupTable_);
    shape_->getMapper()->ScalarVisibilityOn();
    shape_->getMapper()->SetScalarModeToUseCellData();
    shape_->getMapper()->SetColorModeToMapScalars();
    shape_->getMapper()->SetScalarRange(range[0], range[1]);
    shape_->getMapper()->ScalarVisibilityOn();
    shape_->getMapper()->Modified();
    
    shape_->getPolyDataNormals()->Update();
    shape_->getPolyDataNormals()->Modified();
    
    shape_->getRenderer()->GetRenderWindow()->Render();
    
}