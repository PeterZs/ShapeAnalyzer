#include "FunctionTransferTab.h"

///////////////////////////////////////////////////////////////////////////////
custom::tabs::FunctionTransferTab::~FunctionTransferTab() {
}


///////////////////////////////////////////////////////////////////////////////
custom::tabs::FunctionTransferTab::FunctionTransferTab(
                                const HashMap<vtkActor*, shared_ptr<Shape>>& shapes,
                                const HashMap<shared_ptr<PointCorrespondence>, bool>& pointCorrespondences,
                                const HashMap<shared_ptr<FaceCorrespondence>, bool>& faceCorrespondences,
                                ShapeAnalyzerInterface* shapeAnalyzer)
    :   QWidget(dynamic_cast<QWidget*>(shapeAnalyzer), 0),
        CustomTab(shapes, pointCorrespondences, faceCorrespondences, shapeAnalyzer)
{
    this->setupUi(this);

    QStringList labels;
    for(auto entry : shapes_) {

        QString label = QString::number(entry.second->getId());
        label.append(QString::fromStdString(":"+entry.second->getName()));
        labels << label;

    }
    
    if(shapes_.size() < 2) {
        buttonTransfer->setEnabled(false);
    }
    
    comboBoxSourceShape->insertItems(0, labels);
    comboBoxTargetShape->insertItems(0, labels);
    
    connect(this->buttonTransfer,                   SIGNAL(released()),
            this,                                   SLOT(slotTransfer()));
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::FunctionTransferTab::slotTransfer() {
    vtkIdType sid = comboBoxSourceShape->currentText().split(':')[0].toInt();
    vtkIdType tid = comboBoxTargetShape->currentText().split(':')[0].toInt();

    
    shared_ptr<Shape> source = nullptr;
    shared_ptr<Shape> target = nullptr;
    for(auto entry : shapes_) {
        if(sid == entry.second->getId()) {
            source = entry.second;
        }
        
        if(tid == entry.second->getId()) {
            target = entry.second;
        }
    }
    if(source == target) {
        QMessageBox::warning(dynamic_cast<QWidget*>(shapeAnalyzer_), "Error", "The shapes source and target have to be diffrent.");
        return;
    }
    

    
    if(source->getColoring() == nullptr || (source->getColoring()->type != Shape::Coloring::Type::PointScalar && source->getColoring()->type != Shape::Coloring::Type::PointSegmentation)) {
        QMessageBox::warning(dynamic_cast<QWidget*>(shapeAnalyzer_), "Error", "Shape \"" + QString(source->getName().c_str()) + "\" does neither have a scalar point map nor a segmentation. Compute scalar point map or a segmentation first.");
        return;
    }
    
    
    // initialize lists of corresponding contraints on both shapes. Ordering represents correspondence of contraints. I.e. c1[5] on shape1 corresponds to c2[5] on shape2.
    vector<vtkSmartPointer<vtkDoubleArray>> cs; // corresponds to contraints on shape1
    vector<vtkSmartPointer<vtkDoubleArray>> ct;
    
    
    // compute landmark matches using all available correspondences between shape1 and shape2 and geodesic metric
    metric::GeodesicMetric ms(source);
    metric::GeodesicMetric mt(target);

    
    for(auto entry : pointCorrespondences_) {
        shared_ptr<PointCorrespondence> corr = entry.first;
        
        for(int i = 0; i < corr->getShapes().size(); i++) {
            if(corr->getShapes().at(i) == source) {
                vtkSmartPointer<vtkDoubleArray> distances = ms.getAllDistances(corr->getCorrespondingIds().at(i));
                cs.push_back(distances);
                
            }
            
            if(corr->getShapes().at(i) == target) {
                vtkSmartPointer<vtkDoubleArray> distances = mt.getAllDistances(corr->getCorrespondingIds()[i]);
                ct.push_back(distances);
            }
        }
    }
    
    try {
        auto laplacianSource = make_shared<laplaceBeltrami::PetscFEMLaplaceBeltramiOperator>(source, 100);
        auto laplacianTarget = make_shared<laplaceBeltrami::PetscFEMLaplaceBeltramiOperator>(target, 100);
        
        
        // compute 200-dimensional wave kernel discriptor on both shapes
        PetscWaveKernelSignature wksSource(source, 100, laplacianSource);
        
        
        PetscWaveKernelSignature wksTarget(target, 100, laplacianTarget);
        
        // use first 125 components of wave kernel signature as additional constraints. Truncate rest because wave kernel seems to be inaccurate in higher dimensions
        for(int i = 0; i < 10; i++) {
            vtkSmartPointer<vtkDoubleArray> wksiSource = wksSource.getComponent(i);
            cs.push_back(wksiSource);
            
            vtkSmartPointer<vtkDoubleArray> wksiTarget = wksTarget.getComponent(i);
            ct.push_back(wksiTarget);
        }
        
        // compute correspondence matrix C
        PetscFunctionalMaps functionalMaps(source, target, laplacianSource, laplacianTarget, cs, ct, 100);
        
        
        // transfer the coordinate function
        vtkSmartPointer<vtkDoubleArray> Tf = functionalMaps.transferFunction(source->getColoring()->values);
        
        // color 2nd shape
        shared_ptr<Shape::Coloring> coloring = make_shared<Shape::Coloring>();
        coloring->type = Shape::Coloring::Type::PointScalar;
        coloring->values = Tf;
        target->setColoring(coloring);
    } catch(metric::MetricError& me) {
        QErrorMessage msgBox;
        msgBox.showMessage(QString::fromStdString(me.what()));
        msgBox.exec();
    } catch(laplaceBeltrami::LaplaceBeltramiError& le) {
        QErrorMessage msgBox;
        msgBox.showMessage(QString::fromStdString(le.what()));
        msgBox.exec();
    }
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::FunctionTransferTab::onShapeAdd(Shape* shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBoxSourceShape->insertItem(0, label);
    comboBoxTargetShape->insertItem(0, label);
    this->buttonTransfer->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::FunctionTransferTab::onShapeDelete(Shape* shape) {
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
    if(comboBoxSourceShape->count() == 0) {
        this->buttonTransfer->setEnabled(false);
    }
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::FunctionTransferTab::onShapeEdit(Shape* shape) {
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
void custom::tabs::FunctionTransferTab::onClear() {
    this->comboBoxSourceShape->clear();
    this->comboBoxTargetShape->clear();
    this->buttonTransfer->setEnabled(false);
}
