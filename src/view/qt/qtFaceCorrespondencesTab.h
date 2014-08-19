//
//  qtCorrespondenceTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#ifndef __ShapeAnalyzer__qtFaceCorrespondencesTab__
#define __ShapeAnalyzer__qtFaceCorrespondencesTab__

#include <QAction>
#include <QDialog>
#include <QMainWindow>
#include <QMenu>
#include <QPoint>
#include <QString>
#include <QWidget>

#include "../../domain/HashMap.h"
#include "../../domain/correspondences/FaceCorrespondenceData.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"

#include "qtListWidgetItem.h"

#include "../ShapeAnalyzer.h"

#include "ui_faceCorrespondences.h"

using namespace std;

class qtFaceCorrespondencesTab : public QDialog, public Ui::FaceCorrespondencesWidget {
    Q_OBJECT
    
public:
    // copy constructor
    qtFaceCorrespondencesTab(const qtFaceCorrespondencesTab& copy) {
        faceCorrespondences_ = copy.getFaceCorrespondences();
    }
    
    // normal constructor
    qtFaceCorrespondencesTab(
                           HashMap<FaceCorrespondenceData*, bool>*  faceCorrespondences,
                           HashMap<vtkActor*, FaceCorrespondence*>* visibleFaceCorrespondences,
                           HashMap<vtkActor*, Shape*>* shapes,
                           QListWidget* visibleFaceCorrespondencesList,
                           QAction* pointMode,
                           vtkSmartPointer<vtkRenderer> renderer,
                           QWidget * parent = 0,
                           Qt::WindowFlags f = 0
                           );
    ~qtFaceCorrespondencesTab() {}

    
    HashMap<FaceCorrespondenceData*, bool>* getFaceCorrespondences() const  {
        return faceCorrespondences_;
    }
    
private slots:
    virtual void slotOpenContextMenu(const QPoint& point);

    virtual void slotClear();
    
    virtual void slotRandomSubset();
    
private:
    
    HashMap<FaceCorrespondenceData*, bool>*     faceCorrespondences_;
    HashMap<vtkActor*, FaceCorrespondence*>*    visibleFaceCorrespondences_;
    HashMap<vtkActor*, Shape*>*                 shapes_;
    QListWidget*                                visibleFaceCorrespondencesList_;
    QAction*                                    pointMode_;
    vtkSmartPointer<vtkRenderer>                renderer_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceTab__) */
