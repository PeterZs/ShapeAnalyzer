//
//  CorrespondenceColoring.h
//  ShapeAnalyzer
//
// Colors the given shapes to visualize the given correspondence.
// The first shape in the vector will be colored by coordinates and
// all points with a corresponding point on another shape will be colored
// in the same way as the correspondence point
//
//  Created by Zorah on 08.07.14.
//
//

#ifndef __ShapeAnalyzer__CorrespondenceColoring__
#define __ShapeAnalyzer__CorrespondenceColoring__

#include "ShapeFaceColoring.h"
#include "ShapePointColoring.h"

#include "../Shape.h"

using namespace std;

class CorrespondenceColoring : public ShapeFaceColoring, public ShapePointColoring {
public:
    CorrespondenceColoring(vector<Shape*> shapes);
    
protected:
    vector<Shape*> shapes_;
};

#endif /* defined(__ShapeAnalyzer__CorrespondenceColoring__) */
