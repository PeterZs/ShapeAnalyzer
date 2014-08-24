//
//  qtShapeInfoTab.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#ifndef __ShapeAnalyzer__qtShapeInfoTab__
#define __ShapeAnalyzer__qtShapeInfoTab__

#include <iostream>

#include <QPushbutton>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>

#include "../../domain/Shape.h"

#include "qtListWidgetItem.h"
#include "qtShapesTab.h"

#include "ui_shapeInfo.h"


class qtShapeInfoTab : public QWidget, private Ui::ShapeInfoWidget, public qtShapesTab {
    Q_OBJECT
    
public:
    qtShapeInfoTab(QWidget *parent, Qt::WindowFlags f = 0);
    qtShapeInfoTab(Shape* shape, QWidget *parent, Qt::WindowFlags f = 0);
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onShapeSelect(Shape* shape);
    virtual void onClear();
    
private:
    void initialize();
};

#endif /* defined(__ShapeAnalyzer__qtShapeInfoTab__) */
