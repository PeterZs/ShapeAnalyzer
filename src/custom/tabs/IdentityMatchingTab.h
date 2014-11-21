//
//  IdentityMatchingTab.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#ifndef __ShapeAnalyzer__IdentityMatchingTab__
#define __ShapeAnalyzer__IdentityMatchingTab__

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

#include <QMessageBox>

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../util/HashMap.h"

#include "../../view/ShapeAnalyzerInterface.h"
#include "../../view/CustomListWidgetItem.h"

#include "CustomTab.h"

#include "ui_IdentityMatchingTabWidget.h"


class IdentityMatchingTab : public QWidget, private Ui::IdentityMatchingTabWidget, public CustomTab {
    Q_OBJECT
    
public:
    IdentityMatchingTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent);
    
    virtual ~IdentityMatchingTab();
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();
private slots:
    virtual void slotMatch();
};

#endif /* defined(__ShapeAnalyzer__IdentityMatchingTab__) */
