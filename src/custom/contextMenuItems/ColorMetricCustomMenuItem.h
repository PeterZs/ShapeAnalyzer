//
//  ColorMetricCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ColorMetricCustomMenuItem__
#define __ShapeAnalyzer__ColorMetricCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/metric/Metric.h"
#include "../../domain/attributes/ScalarPointAttribute.h"


#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;
using namespace metric;

template<class T>
class ColorMetricCustomMenuItem : public CustomContextMenuItem {
public:
    static const int bla;
    
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
        bool ok;
        vtkIdType source = QInputDialog::getInt(
                                                parent,
                                                "Source vertex",
                                                "Choose ID of source vertex.",
                                                0,
                                                0,
                                                shape->getPolyData()->GetNumberOfPoints()-1,
                                                1,
                                                &ok
                                                );
        
        if (ok) {
            T m(shape);
            ScalarPointAttribute distances(shape);
            m.getAllDistances(distances, source);

            shape->colorPointsScalars(distances.getScalars());
        }
    }
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<ColorMetricCustomMenuItem<T>>(new ColorMetricCustomMenuItem<T>());
    }
    
private:
    ColorMetricCustomMenuItem<T>() {}
};

#endif /* defined(__ShapeAnalyzer__ColorMetricCustomMenuItem__) */
