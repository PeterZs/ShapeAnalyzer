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
#include "qtCorrespondencesTab.h"

#include "../ShapeAnalyzer.h"

#include "ui_faceCorrespondences.h"

class ShapeAnalyzer;

using namespace std;

class qtFaceCorrespondencesTab : public QWidget, public Ui::FaceCorrespondencesWidget, public qtCorrespondencesTab {
    Q_OBJECT
    
public:
    qtFaceCorrespondencesTab(
                             HashMap<FaceCorrespondenceData*, bool>* faceCorrespondences,
                             ShapeAnalyzer* parent,
                             Qt::WindowFlags f = 0
                             );
    ~qtFaceCorrespondencesTab() {}

    virtual void onClear();
    virtual void onShapeDelete(Shape* shape);
    //virtual void onCorrespondencesAddAll(vector<CorrespondenceData*>& correspondences);
    virtual void onCorrespondenceAdd(CorrespondenceData* correspondence);
    virtual void onCorrespondenceDelete(CorrespondenceData* correspondence);
    virtual void onCorrespondenceEdit(Correspondence* correspondence);
    virtual void onCorrespondenceSelect(Correspondence* correspondence);
    virtual void onPointCorrespondencesClear();
    virtual void onFaceCorrespondencesClear();

private slots:
    virtual void slotOpenContextMenu(const QPoint& point);
    
    virtual void slotClear();
    
    virtual void slotSample();
    
    virtual void slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous);
    
private:
    HashMap<FaceCorrespondenceData*, bool>*     faceCorrespondences_;
    ShapeAnalyzer*                              parent_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceTab__) */
