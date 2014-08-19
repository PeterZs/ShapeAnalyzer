//
//  qtCorrespondenceColoringTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

#ifndef __ShapeAnalyzer__qtCorrespondenceColoringTab__
#define __ShapeAnalyzer__qtCorrespondenceColoringTab__

#include <QListWidget.h>
#include <QString.h>
#include <QWidget.h>

#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkType.h>

#include "qtListWidgetItem.h"

#include "../../domain/HashMap.h"
#include "../../domain/Shape.h"

#include "../../domain/correspondences/FaceCorrespondenceData.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"

#include "../../domain/coloring/CorrespondenceColoring.h"

#include "ui_correspondenceColoring.h"

class qtCorrespondenceColoringTab : public QWidget, public Ui::CorrespondenceColoringWidget {
    Q_OBJECT
    
public:
    qtCorrespondenceColoringTab(QListWidget*                            shapes,
                                HashMap<vtkActor*, Shape*>*                 map,
                                HashMap<FaceCorrespondenceData*, bool>*     faceCorr,
                                HashMap<PointCorrespondenceData*, bool>*    pointCorr,
                                QWidget * parent,
                                Qt::WindowFlags f = 0);
    
private slots:
    virtual void slotColorCorrespondences(QString);
    
private:
    QListWidget*                            shapes_;
    HashMap<vtkActor*, Shape*>*                 map_;
    HashMap<FaceCorrespondenceData*, bool>*     faceCorr_;
    HashMap<PointCorrespondenceData*, bool>*    pointCorr_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceColoringTab__) */
