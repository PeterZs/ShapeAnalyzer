//
//  qtShapeInfoTab.cxx
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "qtShapeInfoTab.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
qtShapeInfoTab::qtShapeInfoTab(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    initialize();
}


///////////////////////////////////////////////////////////////////////////////
qtShapeInfoTab::qtShapeInfoTab(Shape* shape, QWidget *parent, Qt::WindowFlags f) {
    initialize();
    
    fillInInformation(shape);
}


///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::initialize() {
    this->setupUi(this);
    
    tableWidget->setRowCount(5);
    tableWidget->setColumnCount(1);
    
    
    tableWidget->horizontalHeader()->hide();
    
    QStringList tableVerticalHeader;
    tableVerticalHeader<<"ID"<<"Name"<<"#Vertices"<<"#Faces"<<"Area";
    tableWidget->setVerticalHeaderLabels(tableVerticalHeader);
}


///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::fillInInformation(Shape* shape) {
    //insert data
    
    // shape id
    tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(shape->getId())));
    // shape name
    tableWidget->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(shape->getName())));
    // number of vertices
    tableWidget->setItem(2, 0, new QTableWidgetItem(
                                                    QString::number(
                                                                    (int) shape->getPolyData()->GetPoints()->GetNumberOfPoints()
                                                                    )
                                                    ));
    // number of cells
    tableWidget->setItem(3, 0, new QTableWidgetItem(
                                                    QString::number(
                                                                    (int) shape->getPolyData()->GetNumberOfCells()
                                                                    )
                                                    )
                         );
    // area of the shape
    tableWidget->setItem(4, 0, new QTableWidgetItem(
                                                    QString::number(
                                                                    (double) shape->getArea()
                                                                    )
                                                    )
                         );
}


///////////////////////////////////////////////////////////////////////////////
// qtTab / qtShapesTab Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::onShapeAdd(Shape* shape) {
    ;
}


///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::onShapeDelete(Shape* shape) {
    if(tableWidget->item(0, 0)->text().toInt() == shape->getId()) {
        tableWidget->clearContents();
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::onShapeEdit(Shape* shape) {
    if(tableWidget->item(0, 0)->text().toInt() == shape->getId()) {
        // update name
        tableWidget->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(shape->getName())));
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::onShapeSelect(Shape* shape) {
    fillInInformation(shape);
}


///////////////////////////////////////////////////////////////////////////////
void qtShapeInfoTab::onClear() {
    tableWidget->clearContents();
}


