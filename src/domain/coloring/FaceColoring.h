//
//  ShapeFaceColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef ShapeAnalyzer_FaceColoring_h
#define ShapeAnalyzer_FaceColoring_h


class FaceColoring {
public:
    virtual ~FaceColoring() {};
    
    virtual void color() = 0;
};


#endif
