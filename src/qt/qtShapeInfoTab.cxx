//
//  qtShapeInfoTab.cxx
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "qtShapeInfoTab.h"

// will create widget with empty table
qtShapeInfoTab::qtShapeInfoTab() {
    QTableView *table = new QTableView( this );
    
    QStandardItemModel *model = new QStandardItemModel(5, 1, this);
    
    table->setModel(model);
}

// will create Widget with basic information about the given shape
qtShapeInfoTab::qtShapeInfoTab( ShapeListItem *item, QWidget *parent, Qt::WindowFlags f )
: QWidget( parent, f )
{
    
    QTableView *table = new QTableView( this );
    
    QStandardItemModel *model = new QStandardItemModel(5, 1, this);
    model->setVerticalHeaderItem(0, new QStandardItem(QString("Name")));
    model->setVerticalHeaderItem(1, new QStandardItem(QString("#Vertices")));
    model->setVerticalHeaderItem(2, new QStandardItem(QString("#Faces")));
    model->setVerticalHeaderItem(3, new QStandardItem(QString("Area")));
    model->setVerticalHeaderItem(4, new QStandardItem(QString("Diameter")));
    
    model->setItem(0, 0, new QStandardItem(item->text()));
    
    model->setItem(1, 0,
                   new QStandardItem(
                                     QString::number(
                                                (int) item->getShape()->getPolyData()->GetPoints()->GetNumberOfPoints()
                                             )
                                     )
                   );
    model->setItem(2, 0, new QStandardItem(
                                           QString::number(
                                                   (int) item->getShape()->getPolyData()->GetNumberOfCells()
                                                   )
                                           )
                   );
    
    table->setModel(model);
    
    
}