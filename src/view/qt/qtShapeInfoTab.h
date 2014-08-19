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

#include "ui_shapeInfo.h"


class qtShapeInfoTab : public QWidget, private Ui::ShapeInfoWidget {
    Q_OBJECT
    
public:
    qtShapeInfoTab(QWidget *parent, Qt::WindowFlags f = 0);
    qtShapeInfoTab(qtListWidgetItem<Shape> *item, QWidget *parent = 0, Qt::WindowFlags f = 0);
    
private:
    void qtSetupTableWidget();
};

#endif /* defined(__ShapeAnalyzer__qtShapeInfoTab__) */
