//
//  ShapeFaceColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_ShapeFaceColoring_h
#define ShapeAnalyzer_ShapeFaceColoring_h

using namespace std;

class ShapeFaceColoring {
public:
    virtual ~ShapeFaceColoring() {};
    
    virtual void colorShapeFaces() = 0;
};


#endif
