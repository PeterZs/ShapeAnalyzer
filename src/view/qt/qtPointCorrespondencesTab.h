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
#include "qtCorrespondencesTab.h"

#include "../ShapeAnalyzer.h"

#include "ui_pointCorrespondences.h"

class ShapeAnalyzer;

using namespace std;

class qtPointCorrespondencesTab : public QWidget, private Ui::PointCorrespondencesWidget, public qtCorrespondencesTab {
    Q_OBJECT
    
public:
    qtPointCorrespondencesTab(
                           HashMap<PointCorrespondenceData*, bool>* pointCorrespondences,
                           ShapeAnalyzer* parent,
                           Qt::WindowFlags f = 0
                           );
    ~qtPointCorrespondencesTab() {}

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
    
    HashMap<PointCorrespondenceData*, bool>*    pointCorrespondences_;
    ShapeAnalyzer*                              parent_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceTab__) */
