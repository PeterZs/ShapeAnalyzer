//
//  CorrespondenceColoringTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

#ifndef __ShapeAnalyzer__CorrespondenceColoringTab__
#define __ShapeAnalyzer__CorrespondenceColoringTab__

#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QListWidget>
#include <QString>
#include <QStringList>
#include <QWidget>

#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkType.h>

#include "../../view/CustomListWidgetItem.h"
#include "CustomTab.h"

#include "../../util/HashMap.h"
#include "../../domain/Shape.h"

#include "../../domain/correspondences/FaceCorrespondenceData.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"

#include "CorrespondenceColoring.h"

#include "ui_CorrespondenceColoringTabWidget.h"

using namespace coloring;

class CorrespondenceColoringTab : public QWidget, public Ui::CorrespondenceColoringTabWidget, public CustomTab {
    Q_OBJECT
    
public:
    CorrespondenceColoringTab(const HashMap<vtkActor*, Shape*>& shapes,
                              const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                              const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences,
                              QWidget* parent);
    
    virtual ~CorrespondenceColoringTab() {}
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();
    
    static CustomTab* create(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent) {
        return new CorrespondenceColoringTab(shapes, pointCorrespondences, faceCorrespondences, parent);
    }
    
private slots:
    virtual void slotColorCorrespondences(QString);
    
private:
    void clearGrid();
};


#endif /* defined(__ShapeAnalyzer__CorrespondenceColoringTab__) */
