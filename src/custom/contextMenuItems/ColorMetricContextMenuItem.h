//
//  ColorMetricContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorMetricContextMenuItem__
#define __ShapeAnalyzer__ColorMetricContextMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/metric/Metric.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;
using namespace metric;

template<class T = Metric>
class ColorMetricContextMenuItem : public CustomContextMenuItem {
public:
    ColorMetricContextMenuItem<T>(Shape* shape, ShapeAnalyzerInterface* shapeAnalyzer) : CustomContextMenuItem(shape, shapeAnalyzer) {}
    
    virtual void onClick(vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
        bool ok;
        vtkIdType source = QInputDialog::getInt(
                                                parent,
                                                "Source vertex",
                                                "Choose ID of source vertex.",
                                                0,
                                                0,
                                                shape_->getPolyData()->GetNumberOfPoints()-1,
                                                1,
                                                &ok
                                                );
        
        if (ok) {
            T m(shape_);
            vtkSmartPointer<vtkDoubleArray> distances = m.getAllDistances(source);

            shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
            coloring->type = Shape::Coloring::Type::PointScalar;
            coloring->values = distances;
            shape_->setColoring(coloring);
        }
    }
};

#endif /* defined(__ShapeAnalyzer__ColorMetricContextMenuItem__) */
