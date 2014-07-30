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
#include <QTableView>
#include <QWidget>

#include "../../domain/Shape.h"

#include "qtListWidgetItem.h"

class qtShapeInfoTab : public QWidget {

public:
    qtShapeInfoTab();
    qtShapeInfoTab(qtListWidgetItem<Shape> *item, QWidget *parent = 0, Qt::WindowFlags f = 0 );
};

#endif /* defined(__ShapeAnalyzer__qtShapeInfoTab__) */
