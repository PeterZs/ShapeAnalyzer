#include "ShapeInterpolationTab.h"

///////////////////////////////////////////////////////////////////////////////
custom::tabs::ShapeInterpolationTab::~ShapeInterpolationTab() {
}


///////////////////////////////////////////////////////////////////////////////
custom::tabs::ShapeInterpolationTab::ShapeInterpolationTab(
                                const HashMap<vtkActor*, shared_ptr<Shape>>& shapes,
                                const HashMap<shared_ptr<PointCorrespondence>, bool>& pointCorrespondences,
                                const HashMap<shared_ptr<FaceCorrespondence>, bool>& faceCorrespondences,
                                ShapeAnalyzerInterface* shapeAnalyzer)
    :   QWidget(dynamic_cast<QWidget*>(shapeAnalyzer), 0),
        CustomTab(shapes, pointCorrespondences, faceCorrespondences, shapeAnalyzer),
        source_(nullptr),
        target_(nullptr),
        shape_(nullptr)
{
    this->setupUi(this);
    
    QStringList labels;
    for(auto entry : shapes_) {

        QString label = QString::number(entry.second->getId());
        label.append(QString::fromStdString(":"+entry.second->getName()));
        labels << label;

    }
    
    if(shapes_.size() < 1) {
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


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::slotChooseShapes() {
    this->labelChoose->setEnabled(false);
    this->comboBoxSourceShape->setEnabled(false);
    this->comboBoxTargetShape->setEnabled(false);
    this->buttonChoose->setEnabled(false);
    
    vtkIdType sid = comboBoxSourceShape->currentText().split(':')[0].toInt();
    vtkIdType tid = comboBoxTargetShape->currentText().split(':')[0].toInt();

    
    for(auto entry : shapes_) {
        if(sid == entry.second->getId()) {
            source_ = entry.second;
        }
        
        if(tid == entry.second->getId()) {
            target_ = entry.second;
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
    
    shape_ = shapeAnalyzer_->addShape(name, polyData);

    
    this->labelInterpolation->setEnabled(true);
    this->sliderInterpolation->setEnabled(true);
    this->sliderInterpolation->setValue(0);
    this->buttonInterpolation->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::slotAddShape() {
    this->labelInterpolation->setEnabled(false);
    this->sliderInterpolation->setEnabled(false);
    this->buttonInterpolation->setEnabled(false);
    
    this->labelChoose->setEnabled(true);
    this->comboBoxSourceShape->setEnabled(true);
    this->comboBoxTargetShape->setEnabled(true);
    this->buttonChoose->setEnabled(true);
    

    shape_->modified();

    shape_ = nullptr;
    source_ = nullptr;
    target_ = nullptr;
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::slotInterpolate(int value) { // value lies between 0 and 100.
    double lambda = (double) value / 100.0;


    // for each pointCorrespondence of the two shapes compute convex combination c of the corresponding points a and b.
    for(auto entry : pointCorrespondences_) {
        
        double a[3];
        double b[3];
        
        vtkIdType pointId; // id of point "a" of source shape that is going to be replaced by c
        
        bool foundSource = false;
        bool foundTarget = false;
        for(int i = 0; i < entry.first->getShapes().size(); i++) {
            if(source_ == entry.first->getShapes().at(i)) {
                foundSource = true;
                pointId = entry.first->getCorrespondingIds().at(i);
                source_->getPolyData()->GetPoint(pointId, a);
            }
            if(target_ == entry.first->getShapes().at(i)) {
                foundTarget = true;
                target_->getPolyData()->GetPoint(entry.first->getCorrespondingIds().at(i), b);
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
        
        //modify polyData
        shape_->getPolyData()->GetPoints()->SetPoint(pointId, c);
    }
    
    // Tell the shape that polyData has been modified. Trigger recomputation of bounding box and rerendering
    shape_->modified();
    
    shapeAnalyzer_->render();
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::onShapeAdd(Shape* shape) {
    this->buttonChoose->setEnabled(true);
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBoxSourceShape->insertItem(0, label);
    comboBoxTargetShape->insertItem(0, label);
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::onShapeDelete(Shape* shape) {
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
    
    if((shape_ != nullptr && shape_->getId() == shape->getId()) || (source_ != nullptr && source_->getId() == shape->getId()) || (target_ != nullptr && target_->getId() == shape->getId())) {
        this->labelInterpolation->setEnabled(false);
        this->sliderInterpolation->setEnabled(false);
        this->buttonInterpolation->setEnabled(false);
        
        this->labelChoose->setEnabled(true);
        this->comboBoxSourceShape->setEnabled(true);
        this->comboBoxTargetShape->setEnabled(true);
        this->buttonChoose->setEnabled(shapes_.size() > 1);
        
        
        shape_ = nullptr;
        source_ = nullptr;
        target_ = nullptr;
    }
    
    if(this->buttonChoose->isEnabled() && shapes_.size() == 1) {
        this->buttonChoose->setEnabled(false);
    }
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::onShapeEdit(Shape* shape) {
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


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::ShapeInterpolationTab::onClear() {
    this->comboBoxSourceShape->clear();
    this->comboBoxTargetShape->clear();
    this->labelInterpolation->setEnabled(false);
    this->sliderInterpolation->setEnabled(false);
    this->buttonInterpolation->setEnabled(false);
    
    this->labelChoose->setEnabled(true);
    this->comboBoxSourceShape->setEnabled(true);
    this->comboBoxTargetShape->setEnabled(true);
    this->buttonChoose->setEnabled(false);
    
    shape_ = nullptr;
    source_ = nullptr;
    target_ = nullptr;
}
