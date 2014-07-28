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

#include "../CorrespondenceSet.h"
#include "../FaceCorrespondenceData.h"
#include "../PointCorrespondenceData.h"

#include "qtListWidgetItem.h"

#include "ui_correspondences.h"

using namespace std;

class qtCorrespondenceWindow : public QDialog, private Ui::CorrespondenceWindow {
    Q_OBJECT
    
public:
    // copy constructor
    qtCorrespondenceWindow(const qtCorrespondenceWindow& copy) {
        pointCorr_ = copy.getPointCorrespondences();
        faceCorr_ = copy.getFaceCorrespondences();
    }
    
    // normal constructor
    qtCorrespondenceWindow(
                           CorrespondenceSet<PointCorrespondenceData>* points,
                           CorrespondenceSet<FaceCorrespondenceData>*  faces,
                           QWidget * parent = 0,
                           Qt::WindowFlags f = 0
                           );
    ~qtCorrespondenceWindow() {}
    
    CorrespondenceSet<PointCorrespondenceData>* getPointCorrespondences() const  {
        return pointCorr_;
    }
    
    CorrespondenceSet<FaceCorrespondenceData>* getFaceCorrespondences() const  {
        return faceCorr_;
    }
    
private slots:
    virtual void slotOpenContextMenuPoints(const QPoint& point);
    virtual void slotOpenContextMenuFaces(const QPoint& point);
    
private:
    CorrespondenceSet<PointCorrespondenceData>* pointCorr_;
    CorrespondenceSet<FaceCorrespondenceData>*  faceCorr_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceWindow__) */
