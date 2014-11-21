//
//  MeshCheckTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

#ifndef __ShapeAnalyzer__MeshCheckTab__
#define __ShapeAnalyzer__MeshCheckTab__

#include <QListWidget>
#include <QString>
#include <QWidget>

#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkType.h>

#include "../../util/HashMap.h"
#include "../../domain/MeshChecker.h"
#include "../../domain/Shape.h"

#include "CustomTab.h"

#include "ui_MeshCheckTabWidget.h"

class MeshCheckTab : public QWidget, public Ui::MeshCheckTabWidget, public CustomTab {
    Q_OBJECT
    
public:
    MeshCheckTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent);
    
    virtual ~MeshCheckTab() {}
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();
private slots:
    virtual void slotCheckMesh();
private:
    /// \brief Fills the ComboBox with the names of all shapes.
    /// \details If an entry with the text of currentSelection is created, this entry will be selected. Otherwise the blank entry will be selected.
    void setUpComboBox();
};


#endif /* defined(__ShapeAnalyzer__MeshCheckTab__) */
