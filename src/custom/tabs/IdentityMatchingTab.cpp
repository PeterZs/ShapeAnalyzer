//
//  IdentityMatchingTab.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#include "IdentityMatchingTab.h"

///////////////////////////////////////////////////////////////////////////////
IdentityMatchingTab::~IdentityMatchingTab() {
}


///////////////////////////////////////////////////////////////////////////////
IdentityMatchingTab::IdentityMatchingTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, ShapeAnalyzerInterface* shapeAnalyzer) : QWidget(dynamic_cast<QWidget*>(shapeAnalyzer), 0), CustomTab(shapes, pointCorrespondences, faceCorrespondences, shapeAnalyzer) {
    this->setupUi(this);
    
    QStringList labels;
    for(auto entry : shapes_) {

        QString label = QString::number(entry.second->getId());
        label.append(QString::fromStdString(":"+entry.second->getName()));
        labels << label;

    }
    
    if(shapes_.size() < 2) {
        buttonMatch->setEnabled(false);
    }
    
    comboBoxShape1->insertItems(0, labels);
    comboBoxShape2->insertItems(0, labels);
    
    connect(this->buttonMatch,                     SIGNAL(released()),
            this,                                  SLOT(slotMatch()));
}


///////////////////////////////////////////////////////////////////////////////
void IdentityMatchingTab::slotMatch() {
    vtkIdType sid1 = comboBoxShape1->currentText().split(':')[0].toInt();
    vtkIdType sid2 = comboBoxShape2->currentText().split(':')[0].toInt();

    
    Shape* shape1 = nullptr;
    Shape* shape2 = nullptr;
    
    for(auto entry : shapes_) {
        if(sid1 == entry.second->getId()) {
            shape1 = entry.second;
        }
        
        if(sid2 == entry.second->getId()) {
            shape2 = entry.second;
        }
    }
    if(shape1 == shape2) {
        QMessageBox::warning(dynamic_cast<QWidget*>(shapeAnalyzer_), "Error", "The two shapes \"" + QString(shape1->getName().c_str()) + "\" and \"" + QString(shape2->getName().c_str()) + "\" have to be different.");
        return;
    }

    
    int step = ceil(100.0 / spinBoxPercentage->value());
    
    int type = comboBoxCorrespondenceType->currentIndex();
    if(type == 0) {
        for(int i = 0; i < min(shape1->getPolyData()->GetNumberOfPoints(), shape2->getPolyData()->GetNumberOfPoints()); i+=step) {
            vector<pair<Shape*, vtkIdType>> correspondence;
            correspondence.push_back(make_pair(shape1, i));
            correspondence.push_back(make_pair(shape2, i));
            
            shapeAnalyzer_->addCorrespondence(correspondence, typeid(PointCorrespondenceData));
        }
    } else {
        for(int i = 0; i < min(shape1->getPolyData()->GetNumberOfCells(), shape2->getPolyData()->GetNumberOfCells()); i+=step) {
            vector<pair<Shape*, vtkIdType>> correspondence;
            correspondence.push_back(make_pair(shape1, i));
            correspondence.push_back(make_pair(shape2, i));
            
            shapeAnalyzer_->addCorrespondence(correspondence, typeid(FaceCorrespondenceData));
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void IdentityMatchingTab::onShapeAdd(Shape* shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBoxShape1->insertItem(0, label);
    comboBoxShape2->insertItem(0, label);
    
    this->buttonMatch->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void IdentityMatchingTab::onShapeDelete(Shape* shape) {
    for(int i = comboBoxShape1->count()-1; i >= 0; i--) {
        if(comboBoxShape1->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxShape1->removeItem(i);
            break;
        }
    }
    
    for(int i = comboBoxShape2->count()-1; i >= 0; i--) {
        if(comboBoxShape2->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxShape2->removeItem(i);
            break;
        }
    }
    
    if(comboBoxShape1->count() == 0) {
        this->buttonMatch->setEnabled(false);
    }
}


///////////////////////////////////////////////////////////////////////////////
void IdentityMatchingTab::onShapeEdit(Shape* shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    
    for(int i = comboBoxShape1->count()-1; i >= 0; i--) {
        if(comboBoxShape1->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxShape1->setItemText(i, label);
            break;
        }
    }
    
    for(int i = comboBoxShape2->count()-1; i >= 0; i--) {
        if(comboBoxShape2->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxShape2->setItemText(i, label);
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void IdentityMatchingTab::onClear() {
    this->comboBoxShape1->clear();
    this->comboBoxShape2->clear();

    this->buttonMatch->setEnabled(false);
}
