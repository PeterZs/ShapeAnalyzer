//
//  FPS.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#include "FPS.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor
///////////////////////////////////////////////////////////////////////////////

FPS::FPS(Shape* shape, Metric* metric, bool del, unsigned source) : metric_(metric), del_(del), source_(source) {
    identifier_ = "FPS";
    shape_ = shape;
}

FPS::~FPS() {
    if (del_)
        delete metric_;
}