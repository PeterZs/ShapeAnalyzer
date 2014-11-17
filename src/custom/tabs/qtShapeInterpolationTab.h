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
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../util/HashMap.h"

#include "../../view/ShapeAnalyzer.h"

#include "../../view/CustomListWidgetItem.h"
#include "qtShapesTab.h"
#include "qtCorrespondencesTab.h"

#include "ui_shapeInterpolation.h"

class ShapeAnalyzer;

class qtShapeInterpolationTab : public QWidget, private Ui::ShapeInterpolationWidget, public qtShapesTab{
    Q_OBJECT
    
public:
    qtShapeInterpolationTab(HashMap<vtkActor*, Shape*> const* shapes, HashMap<PointCorrespondenceData*, bool> const* correspondences, ShapeAnalyzer* parent, Qt::WindowFlags f = 0);
    
    virtual ~qtShapeInterpolationTab();
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onShapeSelect(Shape* shape);
    virtual void onClear();
   
private slots:
    virtual void slotInterpolate(int value);
    virtual void slotChooseShapes();
    virtual void slotAddShape();
    
private:
    Shape* source_;
    Shape* target_;
    Shape* shape_; // interpolated shape.
    HashMap<vtkActor*, Shape*> const* shapes_;
    HashMap<PointCorrespondenceData*, bool> const* correspondences_;

    ShapeAnalyzer* parent_;
};

#endif /* defined(__ShapeAnalyzer__qtShapeInterpolationTab__) */
