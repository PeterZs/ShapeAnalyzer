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

class ShapeAnalyzer;

using namespace std;

class qtFaceCorrespondencesTab : public QDialog, public Ui::FaceCorrespondencesWidget {
    Q_OBJECT
    
public:
    qtFaceCorrespondencesTab(
                             HashMap<FaceCorrespondenceData*, bool>* faceCorrespondences,
                             ShapeAnalyzer* parent,
                             Qt::WindowFlags f = 0
                             );
    ~qtFaceCorrespondencesTab() {}

    
private slots:
    virtual void slotOpenContextMenu(const QPoint& point);

    virtual void slotClear();
    
    virtual void slotSample();
    
private:
    
    HashMap<FaceCorrespondenceData*, bool>*     faceCorrespondences_;
    ShapeAnalyzer*                              parent_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceTab__) */
