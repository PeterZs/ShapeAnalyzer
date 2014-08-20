//
//  qtShapesTab.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 19.08.14.
//
//

#ifndef ShapeAnalyzer_qtShapesTab_h
#define ShapeAnalyzer_qtShapesTab_h

#include "qtTab.h"

class qtShapesTab : public qtTab {
public:
    virtual void onShapeAdd(Shape* shape) = 0;
    virtual void onShapeEdit(Shape* shape) = 0;
    virtual void onShapeSelect(Shape* shape) = 0;
};


#endif
