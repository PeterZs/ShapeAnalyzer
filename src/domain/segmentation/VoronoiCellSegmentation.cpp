//
//  VoronoiCellSegmentation.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#include "VoronoiCellSegmentation.h"

vtkSmartPointer<vtkIdList> VoronoiCellSegmentation::getSegmentation() {
    vtkSmartPointer<vtkIdList> seeds = sampling_->getPoints();
    return metric_->getVoronoiCells(seeds);
}
