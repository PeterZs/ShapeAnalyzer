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

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../util/HashMap.h"

#include "../../view/ShapeAnalyzer.h"
#include "../../view/CustomListWidgetItem.h"

#include "CustomTab.h"

#include "ui_shapeInterpolation.h"

class ShapeAnalyzer;

class ShapeInterpolationTab : public QWidget, private Ui::ShapeInterpolationWidget, public CustomTab {
    Q_OBJECT
    
public:
    ShapeInterpolationTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent);
    
    virtual ~ShapeInterpolationTab();
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();

    static CustomTab* create(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent) {
        return new ShapeInterpolationTab(shapes, pointCorrespondences, faceCorrespondences, parent);
    }
    
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
