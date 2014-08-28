//
//  qtShapeInterpolationTab.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 24.08.14.
//
//

#include "qtShapeInterpolationTab.h"

qtShapeInterpolationTab::~qtShapeInterpolationTab() {
    if(shape_ != nullptr)
        shape_->removeFromRenderer();
    parent_->render();
    delete shape_;
}

qtShapeInterpolationTab::qtShapeInterpolationTab(HashMap<vtkActor*, Shape*>* shapes, HashMap<PointCorrespondenceData*, bool>* correspondences, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, ShapeAnalyzer* parent, Qt::WindowFlags f) : QWidget(parent, f), source_(nullptr), target_(nullptr), shape_(nullptr), shapes_(shapes), correspondences_(correspondences), renderer_(renderer), lastInsertShapeID_(lastInsertShapeID), parent_(parent) {
    this->setupUi(this);
    
    QStringList labels;
    for(HashMap<vtkActor*, Shape*>::iterator it = shapes_->begin(); it != shapes_->end(); it++) {

        QString label = QString::number(it->second->getId());
        label.append(QString::fromStdString(":"+it->second->getName()));
        labels << label;

    }
    
    if(shapes_->size() < 1) {
        buttonChoose->setEnabled(false);
    }
    
    comboBoxSourceShape->insertItems(0, labels);
    comboBoxTargetShape->insertItems(0, labels);
    
    connect(this->sliderInterpolation,              SIGNAL(valueChanged(int)),
            this,                                   SLOT(slotInterpolate(int)));
    connect(this->buttonChoose,                     SIGNAL(released()),
            this,                                   SLOT(slotChooseShapes()));
    connect(this->buttonInterpolation,              SIGNAL(released()),
            this,                                   SLOT(slotAddShape()));
}

void qtShapeInterpolationTab::slotChooseShapes() {
    this->labelChoose->setEnabled(false);
    this->comboBoxSourceShape->setEnabled(false);
    this->comboBoxTargetShape->setEnabled(false);
    this->buttonChoose->setEnabled(false);
    
    vtkIdType sid1 = comboBoxSourceShape->currentText().split(':')[0].toInt();
    vtkIdType sid2 = comboBoxTargetShape->currentText().split(':')[0].toInt();

    
    for(HashMap<vtkActor*, Shape*>::iterator it = shapes_->begin(); it != shapes_->end(); it++) {
        if(sid1 == it->second->getId()) {
            source_ = it->second;
        }
        
        if(sid2 == it->second->getId()) {
            target_ = it->second;
        }
    }
    string name = source_->getName();
    name.append(":");
    name.append(target_->getName());
    
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    
    points->DeepCopy(source_->getPolyData()->GetPoints());
    polys->DeepCopy(source_->getPolyData()->GetPolys());
    
    polyData->SetPoints(points);
    polyData->SetPolys(polys);
    
    shape_ = new Shape(lastInsertShapeID_, name, polyData, renderer_);
    shape_->initialize();
    parent_->vtkAddShape(shape_);
    parent_->render();
    this->labelInterpolation->setEnabled(true);
    this->sliderInterpolation->setEnabled(true);
    this->sliderInterpolation->setValue(0);
    this->buttonInterpolation->setEnabled(true);
}

void qtShapeInterpolationTab::slotAddShape() {
    this->labelInterpolation->setEnabled(false);
    this->sliderInterpolation->setEnabled(false);
    this->buttonInterpolation->setEnabled(false);
    
    this->labelChoose->setEnabled(true);
    this->comboBoxSourceShape->setEnabled(true);
    this->comboBoxTargetShape->setEnabled(true);
    this->buttonChoose->setEnabled(true);
    shape_->getBoxWidget()->PlaceWidget();
    parent_->showShape(shape_);
    shape_ = nullptr;
    source_ = nullptr;
    target_ = nullptr;
}

void qtShapeInterpolationTab::slotInterpolate(int value) { // value lies between 0 and 100.
    double lambda = (double) value / 100.0;


    // for each pointCorrespondence of the two shapes compute convex combination c of the corresponding points a and b.
    for(HashMap<PointCorrespondenceData*, bool>::iterator it = correspondences_->begin(); it != correspondences_->end(); it++) {
        
        double a[3];
        double b[3];
        
        vtkIdType pointId; // id of point "a" of source shape that is going to be replaced by c
        
        bool foundSource = false;
        bool foundTarget = false;
        for(int i = 0; i < it->first->getShapeIds().size(); i++) {
            if(source_->getId() == it->first->getShapeIds()[i]) {
                foundSource = true;
                pointId = it->first->getCorrespondingIds()[i];
                source_->getPolyData()->GetPoint(pointId, a);
            }
            if(target_->getId() == it->first->getShapeIds()[i]) {
                foundTarget = true;
                target_->getPolyData()->GetPoint(it->first->getCorrespondingIds()[i], b);
            }
        }
        
        // if current correspondence does not contain both, source and target jump to next correspondence
        if(!foundSource || !foundTarget) {
            continue;
        }
        
        double c[3];
        c[0] = (1-lambda) * a[0] + lambda * b[0];
        c[1] = (1-lambda) * a[1] + lambda * b[1];
        c[2] = (1-lambda) * a[2] + lambda * b[2];
        
        shape_->getPolyData()->GetPoints()->SetPoint(pointId, c);
    }
    
    shape_->getPolyData()->Modified();
    shape_->getActor()->Modified();
    parent_->render();
}

void qtShapeInterpolationTab::onShapeAdd(Shape* shape) {
    this->buttonChoose->setEnabled(true);
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBoxSourceShape->insertItem(0, label);
    comboBoxTargetShape->insertItem(0, label);
}

void qtShapeInterpolationTab::onShapeDelete(Shape* shape) {
    for(int i = comboBoxSourceShape->count()-1; i >= 0; i--) {
        if(comboBoxSourceShape->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxSourceShape->removeItem(i);
            break;
        }
    }
    
    for(int i = comboBoxTargetShape->count()-1; i >= 0; i--) {
        if(comboBoxTargetShape->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxTargetShape->removeItem(i);
            break;
        }
    }
    
    if((source_ != nullptr && source_->getId() == shape->getId()) || (target_ != nullptr && target_->getId() == shape->getId())) {
        this->labelInterpolation->setEnabled(false);
        this->sliderInterpolation->setEnabled(false);
        this->buttonInterpolation->setEnabled(false);
        
        this->labelChoose->setEnabled(true);
        this->comboBoxSourceShape->setEnabled(true);
        this->comboBoxTargetShape->setEnabled(true);
        this->buttonChoose->setEnabled(shapes_->size() > 1);
        
        shape_->removeFromRenderer();
        delete shape_;
        
        shape_ = nullptr;
        source_ = nullptr;
        target_ = nullptr;
    }
    
    if(this->buttonChoose->isEnabled() && shapes_->size() == 1) {
        this->buttonChoose->setEnabled(false);
    }
}

void qtShapeInterpolationTab::onShapeEdit(Shape* shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    
    for(int i = comboBoxSourceShape->count()-1; i >= 0; i--) {
        if(comboBoxSourceShape->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxSourceShape->setItemText(i, label);
            break;
        }
    }
    
    for(int i = comboBoxTargetShape->count()-1; i >= 0; i--) {
        if(comboBoxTargetShape->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxTargetShape->setItemText(i, label);
            break;
        }
    }
}

void qtShapeInterpolationTab::onShapeSelect(Shape* shape) {
    ;
}

void qtShapeInterpolationTab::onClear() {
    this->comboBoxSourceShape->clear();
    this->comboBoxTargetShape->clear();
    this->labelInterpolation->setEnabled(false);
    this->sliderInterpolation->setEnabled(false);
    this->buttonInterpolation->setEnabled(false);
    
    this->labelChoose->setEnabled(true);
    this->comboBoxSourceShape->setEnabled(true);
    this->comboBoxTargetShape->setEnabled(true);
    this->buttonChoose->setEnabled(false);
    
    if(shape_ != nullptr)
        shape_->removeFromRenderer();
    delete shape_;
    
    shape_ = nullptr;
    source_ = nullptr;
    target_ = nullptr;
}
