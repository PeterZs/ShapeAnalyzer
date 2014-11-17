//
//  VoronoiCellsContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__VoronoiCellsContextMenuItem__
#define __ShapeAnalyzer__VoronoiCellsContextMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/LaplaceBeltramiOperator.h"
#include "../../domain/segmentation/VoronoiCellSegmentation.h"
#include "../../domain/samplings/FarthestPointSampling.h"
#include "../../domain/attributes/ScalarPointAttribute.h"
#include "../../domain/attributes/DiscretePointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

template<class T = Metric>
class VoronoiCellsContextMenuItem : public CustomContextMenuItem {
public:
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
        
        if (!ok) {
            return;
        }
        vtkIdType numberOfSegments = QInputDialog::getInt(
                                                          parent,
                                                          "Number of segments",
                                                          "Choose number of segments",
                                                          0,
                                                          0,
                                                          shape->getPolyData()->GetNumberOfPoints()-1,
                                                          1,
                                                          &ok
                                                          );
        if(ok) {
            T m(shape);
            FarthestPointSampling fps(shape, &m, source, numberOfSegments);
            VoronoiCellSegmentation segmentation(shape, &m, &fps);
            
            // save current segmentation for being able to create new shapes out of the segments
            shape->setSegmentation(segmentation.getSegmentation());
        }
    }
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<VoronoiCellsContextMenuItem>(new VoronoiCellsContextMenuItem());
    }
    
private:
    VoronoiCellsContextMenuItem<T>() {}
};

#endif /* defined(__ShapeAnalyzer__VoronoiCellsContextMenuItem__) */
