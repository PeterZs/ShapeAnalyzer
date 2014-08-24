//
//  qtShapeInterpolationTab.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#ifndef __ShapeAnalyzer__qtShapeInterpolationTab__
#define __ShapeAnalyzer__qtShapeInterpolationTab__

#include <vtkActor.h>

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../domain/HashMap.h"

#include "../ShapeAnalyzer.h"

#include "qtListWidgetItem.h"
#include "qtShapesTab.h"
#include "qtCorrespondencesTab.h"

#include "ui_shapeInterpolation.h"

class ShapeAnalyzer;

class qtShapeInterpolationTab : public QWidget, private Ui::ShapeInterpolationWidget, public qtShapesTab{
    Q_OBJECT
    
public:
    qtShapeInterpolationTab(HashMap<vtkActor*, Shape*>* shapes, HashMap<PointCorrespondenceData*, bool>* correspondences, ShapeAnalyzer* parent, Qt::WindowFlags f = 0);
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onShapeSelect(Shape* shape);
    virtual void onClear();
   
private slots:
    virtual void slotInterpolate(int value);
    
private:
    void initialize();
    
    HashMap<vtkActor*, Shape*>* shapes_;
    HashMap<PointCorrespondenceData*, bool>* correspondences_;
    ShapeAnalyzer* parent_;
};

#endif /* defined(__ShapeAnalyzer__qtShapeInterpolationTab__) */
