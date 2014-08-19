//
//  qtShapeInfoTab.cxx
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "qtShapeInfoTab.h"

// will create widget with empty table
qtShapeInfoTab::qtShapeInfoTab(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    this->setupUi(this);
    
    this->qtSetupTableWidget();
}

void qtShapeInfoTab::qtSetupTableWidget() {
    tableWidget->setRowCount(4);
    tableWidget->setColumnCount(1);
    

    tableWidget->horizontalHeader()->hide();
    
    QStringList tableVerticalHeader;
    tableVerticalHeader<<"Name"<<"#Vertices"<<"#Faces"<<"Area";
    tableWidget->setVerticalHeaderLabels(tableVerticalHeader);
}

// will create Widget with basic information about the given shape
qtShapeInfoTab::qtShapeInfoTab(qtListWidgetItem<Shape> *item, QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    this->setupUi(this);

    this->qtSetupTableWidget();


    //insert data
    tableWidget->setItem(0, 0, new QTableWidgetItem(item->text()));
    tableWidget->setItem(1, 0, new QTableWidgetItem(
                                                 QString::number(
                                                                 (int) item->getItem()->getPolyData()->GetPoints()->GetNumberOfPoints()
                                                                 )
                                                 ));
    tableWidget->setItem(2, 0, new QTableWidgetItem(
                                           QString::number(
                                                           (int) item->getItem()->getPolyData()->GetNumberOfCells()
                                                           )
                                           )
                   );
    
    tableWidget->setItem(3, 0, new QTableWidgetItem(
                                           QString::number(
                                                           (double) item->getItem()->calculateArea()
                                                           )
                                           )
                   );
    
}