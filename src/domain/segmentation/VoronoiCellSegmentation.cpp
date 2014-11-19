//
//  VoronoiCellSegmentation.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#include "VoronoiCellSegmentation.h"
vtkSmartPointer<vtkIntArray> segmentation::VoronoiCellSegmentation::getSegments() {
    vtkSmartPointer<vtkIdList> seeds = sampling_->getPoints();
    return metric_->getVoronoiCells(seeds);
}
