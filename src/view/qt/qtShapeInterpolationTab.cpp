//
//  qtShapeInterpolationTab.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#include "qtShapeInterpolationTab.h"

qtShapeInterpolationTab::qtShapeInterpolationTab(HashMap<vtkActor*, Shape*>& shapes, HashMap<PointCorrespondenceData*, bool>* correspondences, QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), correspondences_(correspondences) {
    this->setupUi(this);
    
    QStringList labels;
    for(HashMap<vtkActor*, Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++) {

        QString label = QString::number(it->second->getId());
        label.append(QString::fromStdString(":"+it->second->getName()));
        labels << label;

    }
    
    comboBoxSourceShape->insertItems(0, labels);
    comboBoxTargetShape->insertItems(0, labels);
    
    connect(this->sliderInterpolation,              SIGNAL(valueChanged(int)),
            this,                                   SLOT(slotInterpolate(int)));
}

void qtShapeInterpolationTab::slotInterpolate(int value) {
    
}

void qtShapeInterpolationTab::onShapeAdd(Shape* shape) {
    ;
}

void qtShapeInterpolationTab::onShapeDelete(Shape* shape) {
    ;
}

void qtShapeInterpolationTab::onShapeEdit(Shape* shape) {
    ;
}

void qtShapeInterpolationTab::onShapeSelect(Shape* shape) {
    ;
}

void qtShapeInterpolationTab::onClear() {
    ;
}
