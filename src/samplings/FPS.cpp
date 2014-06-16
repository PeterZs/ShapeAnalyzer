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

FPS::FPS(Shape* shape, Metric* metric, unsigned number, bool del, unsigned source)
    : metric_(metric), del_(del), source_(source)
{
    identifier_ = "FPS";
    shape_ = shape;
    
    idList_ = vtkSmartPointer<vtkIdList>::New();
    
    // calculate sampling
    if (source_ != -1)
        idList_->InsertNextId(source_);
    else
        idList_->InsertNextId(shape_->getRandomPoint());
    
    for (int i = 1; i < number; i++) {
        // get point furthest to all points in idList_
        idList_->InsertNextId(metric_->getPointFurthestToAllSources(idList_));
    }
    
}

FPS::~FPS() {
    if (del_)
        delete metric_;
}