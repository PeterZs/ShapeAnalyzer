//
//  qtCorrespondenceColoringTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

#ifndef __ShapeAnalyzer__qtMeshCheckTab__
#define __ShapeAnalyzer__qtMeshCheckTab__

#include <QListWidget.h>
#include <QString.h>
#include <QWidget.h>

#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkType.h>

#include "../../domain/HashMap.h"
#include "../../domain/MeshChecker.h"
#include "../../domain/Shape.h"

#include "qtShapesTab.h"

#include "ui_meshcheck.h"

class qtMeshCheckTab : public QWidget, public Ui::MeshCheckWidget, public qtShapesTab {
    Q_OBJECT
    
public:
    qtMeshCheckTab( HashMap<vtkActor*, Shape*>* shapes,
                    QWidget *                   parent,
                    Qt::WindowFlags             f = 0);
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onShapeSelect(Shape* shape);
    virtual void onClear();
    
private slots:
    virtual void slotCheckMesh();
    
    void setUpComboBox(QString current = 0);
    
private:
    HashMap<vtkActor*, Shape*>* shapes_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceColoringTab__) */