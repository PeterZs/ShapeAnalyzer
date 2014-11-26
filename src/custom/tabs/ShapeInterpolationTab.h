//
//  ShapeInterpolationTab.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#ifndef __ShapeAnalyzer__ShapeInterpolationTab__
#define __ShapeAnalyzer__ShapeInterpolationTab__

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkBoxRepresentation.h>

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../util/HashMap.h"

#include "../../view/ShapeAnalyzerInterface.h"
#include "../../view/CustomListWidgetItem.h"

#include "CustomTab.h"

#include "ui_ShapeInterpolationTabWidget.h"


class ShapeInterpolationTab : public QWidget, private Ui::ShapeInterpolationTabWidget, public CustomTab {
    Q_OBJECT
    
public:
    ShapeInterpolationTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent);
    
    virtual ~ShapeInterpolationTab();
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();
private slots:
    virtual void slotInterpolate(int value);
    virtual void slotChooseShapes();
    virtual void slotAddShape();
    
private:
    Shape* source_;
    Shape* target_;
    
    /// Interpolated shape.
    Shape* shape_;
};

#endif /* defined(__ShapeAnalyzer__ShapeInterpolationTab__) */
