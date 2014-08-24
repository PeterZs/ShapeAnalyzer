//
//  qtShapeInterpolationTab.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#include "qtShapeInterpolationTab.h"

qtShapeInterpolationTab::qtShapeInterpolationTab(HashMap<vtkActor*, Shape*>* shapes, HashMap<PointCorrespondenceData*, bool>* correspondences, ShapeAnalyzer* parent, Qt::WindowFlags f) : QWidget(parent, f), shapes_(shapes), correspondences_(correspondences), parent_(parent) {
    this->setupUi(this);
    
    QStringList labels;
    for(HashMap<vtkActor*, Shape*>::iterator it = shapes_->begin(); it != shapes_->end(); it++) {

        QString label = QString::number(it->second->getId());
        label.append(QString::fromStdString(":"+it->second->getName()));
        labels << label;

    }
    
    comboBoxSourceShape->insertItems(0, labels);
    comboBoxTargetShape->insertItems(0, labels);
    
    connect(this->sliderInterpolation,              SIGNAL(valueChanged(int)),
            this,                                   SLOT(slotInterpolate(int)));
}


void qtShapeInterpolationTab::slotInterpolate(int value) { // value lies between 0 and 100.
    double lambda = (double) value / 100.0;

    vtkIdType sid1 = comboBoxSourceShape->currentText().split(':')[0].toInt();
    vtkIdType sid2 = comboBoxTargetShape->currentText().split(':')[0].toInt();
    Shape* shape1 = nullptr;
    Shape* shape2 = nullptr;
    
    for(HashMap<vtkActor*, Shape*>::iterator it = shapes_->begin(); it != shapes_->end(); it++) {
        if(sid1 == it->second->getId()) {
            shape1 = it->second;
        }
        
        if(sid2 == it->second->getId()) {
            shape2 = it->second;
        }
    }
    
    
    // for each pointCorrespondence of the two shapes compute convex combination c of the corresponding points a and b.
    for(HashMap<PointCorrespondenceData*, bool>::iterator it = correspondences_->begin(); it != correspondences_->end(); it++) {
        
        double a[3];
        double b[3];
        
        vtkIdType pointId; // id of point "a" of source shape that is going to be replaced by c
        
        for(int i = 0; i < it->first->getShapeIds().size(); i++) {
            if(sid1 == it->first->getShapeIds()[i]) {
                pointId = it->first->getCorrespondingIds()[i];
                shape1->getOriginalPolyData()->GetPoint(pointId, a);
            }
            if(sid2 == it->first->getShapeIds()[i]) {
                shape2->getPolyData()->GetPoint(it->first->getCorrespondingIds()[i], b);
            }
        }
        
        double c[3];
        c[0] = (1-lambda) * a[0] + lambda * b[0];
        c[1] = (1-lambda) * a[1] + lambda * b[1];
        c[2] = (1-lambda) * a[2] + lambda * b[2];
        
        shape1->getPolyData()->GetPoints()->SetPoint(pointId, c);
    }
    
    shape1->getPolyData()->Modified();
    shape1->getActor()->Modified();
    parent_->render();
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
