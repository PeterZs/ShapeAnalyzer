//
//  qtCorrespondenceWindow.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#ifndef __ShapeAnalyzer__qtCorrespondenceWindow__
#define __ShapeAnalyzer__qtCorrespondenceWindow__

#include <QAction>
#include <QDialog>
#include <QMainWindow>
#include <QMenu>
#include <QPoint>
#include <QString>
#include <QWidget>

#include "../../domain/Set.h"
#include "../../domain/correspondences/FaceCorrespondenceData.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"

#include "qtListWidgetItem.h"

#include "../ShapeAnalyzer.h"

#include "ui_correspondences.h"

using namespace std;

class qtCorrespondenceWindow : public QDialog, public Ui::CorrespondenceWindow {
    Q_OBJECT
    
public:
    // copy constructor
    qtCorrespondenceWindow(const qtCorrespondenceWindow& copy) {
        pointCorr_ = copy.getPointCorrespondences();
        faceCorr_ = copy.getFaceCorrespondences();
    }
    
    // normal constructor
    qtCorrespondenceWindow(
                           Set<PointCorrespondenceData*, bool>* points,
                           Set<FaceCorrespondenceData*, bool>*  faces,
                           Set<vtkActor*, PointCorrespondence*>* visiblePoints,
                           Set<vtkActor*, FaceCorrespondence*>* visibleFaces,
                           Set<vtkActor*, Shape*>* shapes,
                           QListWidget* visibleList,
                           QAction* pointMode,
                           vtkSmartPointer<vtkRenderer> renderer,
                           QWidget * parent = 0,
                           Qt::WindowFlags f = 0
                           );
    ~qtCorrespondenceWindow() {}
    
    Set<PointCorrespondenceData*, bool>* getPointCorrespondences() const  {
        return pointCorr_;
    }
    
    Set<FaceCorrespondenceData*, bool>* getFaceCorrespondences() const  {
        return faceCorr_;
    }
    
private slots:
    virtual void slotOpenContextMenuPoints(const QPoint& point);
    virtual void slotOpenContextMenuFaces(const QPoint& point);
    
    virtual void slotClearCorrespondences();
    
    virtual void slotRandomSubsetPoints();
    virtual void slotRandomSubsetFaces();
    
private:
    void addPointCorrespondence(PointCorrespondenceData*);
    
    Set<PointCorrespondenceData*, bool>*    pointCorr_;
    Set<FaceCorrespondenceData*, bool>*     faceCorr_;
    Set<vtkActor*, PointCorrespondence*>*   visiblePoints_;
    Set<vtkActor*, FaceCorrespondence*>*    visibleFaces_;
    Set<vtkActor*, Shape*>*                 shapes_;
    QListWidget*                            visibleList_;
    QAction*                                pointMode_;
    vtkSmartPointer<vtkRenderer>            renderer_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceWindow__) */
