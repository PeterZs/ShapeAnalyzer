//
//  MetricColoring.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 03.06.14.
//
//

#include "MetricColoring.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
MetricColoring::MetricColoring(Metric* metric, bool del) :
    metric_(metric), del_(del)
{
    table_ = vtkSmartPointer<vtkLookupTable>::New();
    source_ = -1;
}


///////////////////////////////////////////////////////////////////////////////
MetricColoring::~MetricColoring() {
    if(del_) {
        delete metric_;
    }
        
}


///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void MetricColoring::colorShapeFaces() {
    
    // get all distances
    vector<double> distances = prepareColoring();

    // list of colors for vtk
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < metric_->getShape()->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        metric_->getShape()->getPolyData()->GetCellPoints(i, ids);
        
        // assign the distance of the closest point
        double dist = distances[ids->GetId(0)];
        if(dist > distances[ids->GetId(1)])
            dist = distances[ids->GetId(1)];
        if(dist > distances[ids->GetId(2)])
            dist = distances[ids->GetId(2)];
        
        double dcolor[3];
        table_->GetColor(dist, dcolor);
        
        unsigned char color[3];
        for(unsigned int j = 0; j < 3; j++) {
            color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
        }
        
        colors->InsertNextTupleValue(color);
    }
    
    // update vtk
    Shape* shape = metric_->getShape();
    
    shape->getPolyData()->GetCellData()->SetScalars(colors);
    shape->getPolyData()->Modified();
    
    shape->getPolyDataNormals()->Update();
    shape->getPolyDataNormals()->Modified();
    
    shape->getRenderer()->GetRenderWindow()->Render();
    
}

///////////////////////////////////////////////////////////////////////////////
void MetricColoring::colorShapePoints() {
        // nooooothing here
}


///////////////////////////////////////////////////////////////////////////////
void MetricColoring::setColorTable(vtkSmartPointer<vtkLookupTable> table) {
    table_ = table;
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
vector<double> MetricColoring::prepareColoring() {
    // assign random starting point, if source_ unset
    if (source_ == -1)
        source_ = rand() % metric_->getShape()->getPolyData()->GetPoints()->GetNumberOfPoints();
    
    // get all distances
    vector<double> distances = metric_->getAllDistances(source_);
    
    // calculate maximum distance
    double max = 0;
    for(vector<double>::iterator it = distances.begin(); it != distances.end(); it++) {
        // replace current maximum if valid
        if(*it > max)
            max = *it;
    }
    
    // build color lookup table
    table_->SetRange(0, max);
    table_->Build();
    
    return distances;
}