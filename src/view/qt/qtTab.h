//
//  qtTab.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 19.08.14.
//
//

#ifndef ShapeAnalyzer_qtTab_h
#define ShapeAnalyzer_qtTab_h

#include "../../domain/Shape.h"

class qtTab {
public:
    virtual void onShapeDelete(Shape* shape) = 0;
    virtual void onClear() = 0;
};

#endif
