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

class ShapeAnalyzer;

using namespace std;

class qtPointCorrespondencesTab : public QWidget, private Ui::PointCorrespondencesWidget {
    Q_OBJECT
    
public:
    qtPointCorrespondencesTab(
                           HashMap<PointCorrespondenceData*, bool>* pointCorrespondences,
                           ShapeAnalyzer* parent,
                           Qt::WindowFlags f = 0
                           );
    ~qtPointCorrespondencesTab() {}

    
private slots:
    virtual void slotOpenContextMenu(const QPoint& point);
    
    virtual void slotClear();
    
    virtual void slotSample();
    
private:
    
    HashMap<PointCorrespondenceData*, bool>*    pointCorrespondences_;
    ShapeAnalyzer*                              parent_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceTab__) */
