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

    QStringList labels = getShapeIdentifierList();
    
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
    vtkIdType sid = getIdFromIdentifier(comboBoxSourceShape->currentText());
    vtkIdType tid = getIdFromIdentifier(comboBoxTargetShape->currentText());

    
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
        QMessageBox::warning(dynamic_cast<QWidget*>(shapeAnalyzer_), "Error", "Shape \"" + QString(source->getName().c_str()) + "\" is neither colored with a scalar point map nor a segmentation. Compute scalar point map or a segmentation first.");
        return;
    }
    
    try {
        
        // initialize lists of corresponding contraints on both shapes. Ordering represents correspondence of contraints. I.e. c1[5] on shape1 corresponds to c2[5] on shape2.
        // corresponds to contraints on shape1
        vector<vtkSmartPointer<vtkDoubleArray>> constraintsSource;
        
        // corresponds to contraints on shape2
        vector<vtkSmartPointer<vtkDoubleArray>> constraintsTarget;
        
        
        metric::GeodesicMetric metricSource(source);
        metric::GeodesicMetric metricTarget(target);
        
        int numberOfEigenfunctions = max(spinBoxNumberOfEigenfunctions->value(), max(spinBoxWaveKernelSignatureNumberOfEigenfunctions->value(), spinBoxHeatBumpsNumberOfEigenfunctions->value()));
        auto laplacianSource = make_shared<laplaceBeltrami::PetscFEMLaplaceBeltramiOperator>(source, numberOfEigenfunctions);
        auto laplacianTarget = make_shared<laplaceBeltrami::PetscFEMLaplaceBeltramiOperator>(target, numberOfEigenfunctions);
        
        
        // Compute landmark correspondences using all available correspondences between shape1 and shape2 and geodesic metric
        // Compute heat diffusion snapshots using the corresponding points.

        for(auto entry : pointCorrespondences_) {
            shared_ptr<PointCorrespondence> corr = entry.first;
            
            for(int i = 0; i < corr->getShapes().size(); i++) {
                if(corr->getShapes().at(i) == source) {
                    // Heat diffusion
                    if(groupBoxHeatBumps->isChecked()) {
                        vtkSmartPointer<vtkDoubleArray> u0s = vtkSmartPointer<vtkDoubleArray>::New();
                        u0s->SetNumberOfValues(source->getPolyData()->GetNumberOfPoints());
                        for(vtkIdType j = 0; j < source->getPolyData()->GetNumberOfPoints(); j++) {
                            u0s->SetValue(j, 0.0);
                        }
                        u0s->SetValue(corr->getCorrespondingIds().at(i), doubleSpinBoxHeatBumpsInitialValue->value());
                        
                        PetscHeatDiffusion hdSource(source, laplacianSource, u0s);
                        for(double t = doubleSpinBoxHeatBumpsFrom->value(); t <= doubleSpinBoxHeatBumpsTo->value(); t+=doubleSpinBoxHeatBumpsStep->value()) {
                            constraintsSource.push_back(hdSource.getHeat(t));
                        }
                    }
                        
                    // distance functions
                    if(checkBoxGeodesicDistanceFunctions->isChecked()) {
                        vtkSmartPointer<vtkDoubleArray> distances = metricSource.getAllDistances(corr->getCorrespondingIds().at(i));
                        constraintsSource.push_back(distances);
                    }
                }
                
                if(corr->getShapes().at(i) == target) {
                    // Heat diffusion
                    if(groupBoxHeatBumps->isChecked()) {
                        vtkSmartPointer<vtkDoubleArray> u0t = vtkSmartPointer<vtkDoubleArray>::New();
                        u0t->SetNumberOfValues(target->getPolyData()->GetNumberOfPoints());
                        
                        for(vtkIdType j = 0; j < target->getPolyData()->GetNumberOfPoints(); j++) {
                            u0t->SetValue(j, 0.0);
                        }
                        u0t->SetValue(corr->getCorrespondingIds().at(i), doubleSpinBoxHeatBumpsInitialValue->value());
                        
                        
                        PetscHeatDiffusion hdTarget(source, laplacianSource, u0t);
                        for(double t = doubleSpinBoxHeatBumpsFrom->value(); t <= doubleSpinBoxHeatBumpsTo->value(); t+=doubleSpinBoxHeatBumpsStep->value()) {
                            constraintsTarget.push_back(hdTarget.getHeat(t));
                        }
                    }
                    
                    // distance functions
                    if(checkBoxGeodesicDistanceFunctions->isChecked()) {
                        vtkSmartPointer<vtkDoubleArray> distances = metricTarget.getAllDistances(corr->getCorrespondingIds()[i]);
                        constraintsTarget.push_back(distances);
                        
                    }
                }
            }
        }
        

        
        // compute wave kernel discriptor on both shapes
        PetscWaveKernelSignature wksSource(source, spinBoxWaveKernelSignatureComponentsTotalNumber->value(), laplacianSource, spinBoxWaveKernelSignatureNumberOfEigenfunctions->value());
        PetscWaveKernelSignature wksTarget(target, spinBoxWaveKernelSignatureComponentsTotalNumber->value(), laplacianTarget, spinBoxWaveKernelSignatureNumberOfEigenfunctions->value());
        
        
        // add wave kernel compontents to contraint vectors
        if(groupBoxWaveKernelSignature->isChecked()) {
            for(int i = spinBoxWaveKernelSignatureComponentsFrom->value(); i < spinBoxWaveKernelSignatureComponentsTo->value(); i+=spinBoxWaveKernelSignatureComponentsStep->value()) {
                vtkSmartPointer<vtkDoubleArray> wksiSource = wksSource.getComponent(i);
                constraintsSource.push_back(wksiSource);
                
                vtkSmartPointer<vtkDoubleArray> wksiTarget = wksTarget.getComponent(i);
                constraintsTarget.push_back(wksiTarget);
            }
        }
        // compute correspondence matrix C


        function<void(int, double)> iterationCallback = [this](int iteration, double residual)->void {
            lcdNumberCurrentIteration->display(iteration);
            lcdNumberResidual->display(residual);
            
            QCoreApplication::processEvents();
        };
        
        stringstream ss;
        PetscFunctionalMaps functionalMaps(source, target, laplacianSource, laplacianTarget, constraintsSource, constraintsTarget, spinBoxNumberOfEigenfunctions->value(), spinBoxStepSize->value(), doubleSpinBoxSparsityPriorWeight->value(), spinBoxNumberOfIterations->value(), groupBoxOutlierAbsorbtion->isChecked(), doubleSpinBoxOutlierAbsorbtionWeight->value(), iterationCallback, ss);

        
        
        
        
        if(source->getColoring()->type == Shape::Coloring::Type::PointSegmentation) {
            // find out the number of segments
            int numberOfSegments = 0;
            for(vtkIdType i = 0; i < source->getColoring()->values->GetNumberOfTuples(); i++) {
                if(source->getColoring()->values->GetComponent(i, 0) > numberOfSegments) {
                    numberOfSegments = source->getColoring()->values->GetComponent(i, 0);
                }
            }
            numberOfSegments++;
            
            // transfer each segment seperately as an segment indicator function
            vector<vtkSmartPointer<vtkDoubleArray>> components(numberOfSegments);
            for(int i = 0; i < numberOfSegments; i++) {
                vtkSmartPointer<vtkDoubleArray> indicatorFunction = vtkSmartPointer<vtkDoubleArray>::New();
                indicatorFunction->SetNumberOfValues(source->getPolyData()->GetNumberOfPoints());
                // color 2nd shape
                for(vtkIdType j = 0; j < source->getPolyData()->GetNumberOfPoints(); j++) {
                    if(source->getColoring()->values->GetComponent(j, 0) == i) {
                        indicatorFunction->SetValue(j, 1.0);
                    } else {
                        indicatorFunction->SetValue(j, 0.0);
                    }
                }
                vtkSmartPointer<vtkDoubleArray> Tf = functionalMaps.transferFunction(indicatorFunction);
                target->setColoring(Tf, Shape::Coloring::Type::PointScalar);
                shapeAnalyzer_->render();
                this_thread::sleep_for(chrono::milliseconds(2000));
                components.at(i) = Tf;
            }
            
            vtkSmartPointer<vtkIntArray> Tf = vtkSmartPointer<vtkIntArray>::New();
            Tf->SetNumberOfValues(source->getPolyData()->GetNumberOfPoints());
            
            for(vtkIdType j = 0; j < source->getPolyData()->GetNumberOfPoints(); j++) {
                int segment = 0;
                double score = components.at(0)->GetValue(j);
                for(int i = 1; i < numberOfSegments; i++) {
                    if(components.at(i)->GetValue(j) > score) {
                        score = components.at(i)->GetValue(j);
                        segment = i;
                    }
                }
                
                Tf->SetValue(j, segment);
            }
            
            target->setColoring(Tf, Shape::Coloring::Type::PointSegmentation);
        } else {
            // transfer function 
            vtkSmartPointer<vtkDoubleArray> Tf = functionalMaps.transferFunction(source->getColoring()->values);
            // color 2nd shape
            target->setColoring(Tf, Shape::Coloring::Type::PointScalar);
        }
        

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
    
    if(shapes_.size() >= 2) {
        this->buttonTransfer->setEnabled(true);
    }
}


///////////////////////////////////////////////////////////////////////////////
void custom::tabs::FunctionTransferTab::onShapeDelete(Shape* shape) {
    for(int i = comboBoxSourceShape->count()-1; i >= 0; i--) {
        if(getIdFromIdentifier(comboBoxSourceShape->itemText(i)) == shape->getId()) {
            comboBoxSourceShape->removeItem(i);
            break;
        }
    }
    
    for(int i = comboBoxTargetShape->count()-1; i >= 0; i--) {
        if(getIdFromIdentifier(comboBoxTargetShape->itemText(i)) == shape->getId()) {
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
    QString label = getShapeIdentifier(shape);
    
    for(int i = comboBoxSourceShape->count()-1; i >= 0; i--) {
        if(getIdFromIdentifier(comboBoxSourceShape->itemText(i)) == shape->getId()) {
            comboBoxSourceShape->setItemText(i, label);
            break;
        }
    }
    
    for(int i = comboBoxTargetShape->count()-1; i >= 0; i--) {
        if(getIdFromIdentifier(comboBoxTargetShape->itemText(i)) == shape->getId()) {
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
