//
//  MetricColoring.h
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#ifndef __ShapeAnalyzer__MetricColoring__
#define __ShapeAnalyzer__MetricColoring__

#include <iostream>

#include "ShapeFaceColoring.h"
#include "ShapePointColoring.h"

#include "../metric/Metric.h"
#include "../Shape.h"

#include <vtkCellData.h>
#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>

class MetricColoring : public ShapeFaceColoring, ShapePointColoring {
public:
    // del initiates the deletion of the metric object in the destructor
    // of this object, standard is false
    MetricColoring(Metric* metric, bool del = false);
    ~MetricColoring();
    
    void setColorTable(vtkSmartPointer<vtkLookupTable> table);
    
    // abstract functions from ShapeFaceColoring and ShapePointColoring
    virtual void colorShapeFaces();
    virtual void colorShapePoints();
    
private:
    vector<double> prepareColoring();
    
    Metric*                         metric_;
    
    unsigned                        source_;
    vtkSmartPointer<vtkLookupTable> table_;
    
    bool                            del_;
};

#endif /* defined(__ShapeAnalyzer__MetricColoring__) */
