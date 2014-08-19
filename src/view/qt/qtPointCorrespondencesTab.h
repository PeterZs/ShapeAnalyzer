//
//  qtCorrespondenceTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#ifndef __ShapeAnalyzer__qtCorrespondenceTab__
#define __ShapeAnalyzer__qtCorrespondenceTab__

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

#include "ui_pointCorrespondences.h"

using namespace std;

class qtPointCorrespondencesTab : public QWidget, private Ui::PointCorrespondencesWidget {
    Q_OBJECT
    
public:
    // copy constructor
    qtPointCorrespondencesTab(const qtPointCorrespondencesTab& copy) {
        pointCorrespondences_ = copy.getPointCorrespondences();
    }
    
    // normal constructor
    qtPointCorrespondencesTab(
                           HashMap<PointCorrespondenceData*, bool>* pointCorrespondences,
                           HashMap<vtkActor*, PointCorrespondence*>* visiblePointCorrespondences,
                           HashMap<vtkActor*, Shape*>* shapes,
                           QListWidget* visiblePointCorrespondencesList,
                           QAction* pointMode,
                           vtkSmartPointer<vtkRenderer> renderer,
                           QWidget * parent = 0,
                           Qt::WindowFlags f = 0
                           );
    ~qtPointCorrespondencesTab() {}
    
    HashMap<PointCorrespondenceData*, bool>* getPointCorrespondences() const  {
        return pointCorrespondences_;
    }
    
private slots:
    virtual void slotOpenContextMenu(const QPoint& point);
    
    virtual void slotClear();
    
    virtual void slotRandomSubset();
    
private:
    void addPointCorrespondence(PointCorrespondenceData*);
    
    HashMap<PointCorrespondenceData*, bool>*    pointCorrespondences_;
    HashMap<vtkActor*, PointCorrespondence*>*   visiblePointCorrespondences_;
    HashMap<vtkActor*, Shape*>*                 shapes_;
    QListWidget*                                visiblePointCorrespondencesList_;
    QAction*                                    pointMode_;
    vtkSmartPointer<vtkRenderer>                renderer_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceTab__) */
