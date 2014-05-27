//
//  qtShapeInfoTab.cxx
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "qtShapeInfoTab.h"

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
                                     QString(
                                             (char *) item->getShape()->getPolyData()->GetNumberOfVerts()
                                             )
                                     )
                   );
    model->setItem(2, 0, new QStandardItem(
                                           QString(
                                                   (char *) item->getShape()->getPolyData()->GetNumberOfVerts()
                                                   )
                                           )
                   );
    
    table->setModel(model);
    
    
}