//
//  qtTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 08.08.14.
//
//

#ifndef ShapeAnalyzer_qtTab_h
#define ShapeAnalyzer_qtTab_h

class qtTab {
public:
    virtual void onShapeDelete(Shape* shape) = 0;
    virtual void onClear() = 0;
};


#endif
