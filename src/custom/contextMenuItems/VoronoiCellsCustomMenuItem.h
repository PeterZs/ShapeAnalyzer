//
//  VoronoiCellsCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__VoronoiCellsCustomMenuItem__
#define __ShapeAnalyzer__VoronoiCellsCustomMenuItem__

#include <string>

#include "CustomContextMenuItem.h"

#include "../../domain/LaplaceBeltramiOperator.h"
#include "../../domain/segmentation/VoronoiCellSegmentation.h"
#include "../../domain/samplings/FarthestPointSampling.h"
#include "../../domain/attributes/ScalarPointAttribute.h"
#include "../../domain/attributes/DiscretePointAttribute.h"

#include "../Factory.h"

#include <qinputdialog.h>

using namespace std;

template<class T>
class VoronoiCellsCustomMenuItem : public CustomContextMenuItem {
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
            
            VoronoiCellSegmentation* segmentation = (VoronoiCellSegmentation*) Factory<Segmentation>::getInstance()->create("VoronoiCellSegmentation");
            
            Metric* m = Factory<Metric>::getInstance()->create(T::getIdentifier());
            m->initialize(shape);
            FarthestPointSampling* fps = (FarthestPointSampling*) Factory<Sampling>::getInstance()->create("fps");
            fps->setMetric(m);
            fps->setSource(source);
            fps->setNumberOfPoints(numberOfSegments);
            fps->initialize(shape);
            ((VoronoiCellSegmentation*) segmentation)->setMetric(m);
            ((VoronoiCellSegmentation*) segmentation)->setSampling(fps);
            
            segmentation->initialize(shape);
             
            // save current segmentation for being able to create new shapes out of the segments
            //segmentations_.add(shape, segmentation->getSegmentation());
            
            
            vtkSmartPointer<vtkIdList> segments = segmentation->getSegmentation();
            DiscretePointAttribute voronoiCells(shape);
            for(vtkIdType i = 0; i < shape->getPolyData()->GetNumberOfPoints(); i++) {
                voronoiCells.getValues()->SetValue(i, segments->GetId(i));
            }
            
            shape->colorPointsScalars(voronoiCells.getValues());
            
            delete m;
            delete fps;
            delete segmentation;
        }
    }
    
    static CustomContextMenuItem* create() {
        return new VoronoiCellsCustomMenuItem<T>();
    }
    
private:
    VoronoiCellsCustomMenuItem<T>() {}
};

#endif /* defined(__ShapeAnalyzer__VoronoiCellsCustomMenuItem__) */
