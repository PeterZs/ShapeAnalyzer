//
//  FPS.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef __ShapeAnalyzer__FPS__
#define __ShapeAnalyzer__FPS__

#include <iostream>

#include "ShapeSampling.h"

#include "../metric/Metric.h"

class FPS : public ShapeSampling {
public:
    FPS(Shape* shape, Metric* metric, unsigned number, bool del = false, unsigned source = -1);
    ~FPS();
    
private:
    Metric* metric_;
    bool del_;
    unsigned source_;
};

#endif /* defined(__ShapeAnalyzer__FPS__) */
