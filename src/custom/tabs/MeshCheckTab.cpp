////
////  MeshCheckTab.cpp
////  ShapeAnalyzer
////
////  Created by Zorah on 26.08.14.
////
////
//
//#include "MeshCheckTab.h"
//
/////////////////////////////////////////////////////////////////////////////////
//// Constructor
/////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////
//MeshCheckTab::MeshCheckTab(const HashMap<vtkActor*, Shape*>& shapes,
//                           const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
//                           const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, ShapeAnalyzerInterface* shapeAnalyzer)
//: QWidget(dynamic_cast<QWidget*>(shapeAnalyzer), 0), CustomTab(shapes, pointCorrespondences, faceCorrespondences, shapeAnalyzer)
//{
//    this->setupUi(this);
//    
//    setUpComboBox();
//    
//    connect(this->buttonCheckMesh,  SIGNAL(released()),
//            this,                   SLOT(slotCheckMesh()));
//    connect(this->comboBox,         SIGNAL(currentIndexChanged(int)),
//            this->outputField,      SLOT(clear()));
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Slots
/////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////
//void MeshCheckTab::slotCheckMesh() {
//    // clear previous output
//    outputField->clear();
//    
//    // find correct shape
//    Shape* shape;
//    bool shapeFound = false;
//    for (auto entry : shapes_) {
//        if(entry.second->getId() == comboBox->currentText().split(':')[0].toInt()) {
//            shape = entry.second;
//            shapeFound = true;
//            break;
//        }
//    }
//    
//    // return if no shape was found
//    if(!shapeFound)
//        return;
//    
//    // run all selected checks
//    MeshChecker check = MeshChecker(shape);
//    
//    // triangulation check
//    if (checkTriangles->isChecked()) {
//        vector<pair<vtkIdType, vtkIdType> > nonTriangles;
//        bool notTriangulated = check.checkTriangulation(&nonTriangles);
//        
//        if(notTriangulated) {
//            outputField->insertPlainText(QString::fromStdString("- Non-Triangles in form (cell-id, #vertices): "));
//            // list of all border edges
//            for (auto nonTriangle : nonTriangles) {
//                string tupel = "(";
//                tupel.append(to_string(nonTriangle.first));
//                tupel.append(", ");
//                tupel.append(to_string(nonTriangle.second));
//                tupel.append(") ");
//                outputField->insertPlainText(QString::fromStdString(tupel));
//            }
//            outputField->insertHtml(QString::fromStdString("<br />"));
//        } else {
//            outputField->insertPlainText(QString::fromStdString("- All cells are triangles."));
//            outputField->insertHtml(QString::fromStdString("<br />"));
//        }
//    }
//    
//    // number of regions
//    if (checkConnectivity->isChecked()) {
//        int numberRegions = check.checkNumberOfRegions();
//        string regions = "- Number of connected Regions: ";
//        regions.append(to_string(numberRegions));
//        outputField->insertPlainText(QString::fromStdString(regions));
//        outputField->insertHtml(QString::fromStdString("<br />"));
//    }
//    
//    
//    // orientation checking
//    if (checkOrientation->isChecked()) {
//        vector<pair<vtkIdType, vtkIdType> > unorientedEdges;
//        bool isUnoriented = check.checkOrientation(&unorientedEdges);
//        
//        // output
//        if(isUnoriented) {
//            outputField->insertPlainText(QString::fromStdString("- Inconsistent orientation at: "));
//            // list of all border edges
//            for (auto e : unorientedEdges) {
//                string tupel = "(";
//                tupel.append(to_string(e.first));
//                tupel.append(", ");
//                tupel.append(to_string(e.second));
//                tupel.append(") ");
//                outputField->insertPlainText(QString::fromStdString(tupel));
//            }
//            outputField->insertHtml(QString::fromStdString("<br />"));
//        } else {
//            outputField->insertPlainText(QString::fromStdString("- Is consistently oriented."));
//            outputField->insertHtml(QString::fromStdString("<br />"));
//        }
//    }
//    
//    
//    // border checking
//    if (checkBorders->isChecked()) {
//        vector<pair<vtkIdType, vtkIdType> > borders;
//        bool hasBorders = check.checkForBorders(&borders);
//        
//        // output
//        if(hasBorders) {
//            outputField->insertPlainText(QString::fromStdString("- Has Borders: "));
//            // list of all border edges
//            for (auto b : borders) {
//                string tupel = "(";
//                tupel.append(to_string(b.first));
//                tupel.append(", ");
//                tupel.append(to_string(b.second));
//                tupel.append(") ");
//                outputField->insertPlainText(QString::fromStdString(tupel));
//            }
//            outputField->insertHtml(QString::fromStdString("<br />"));
//        } else {
//            outputField->insertPlainText(QString::fromStdString("- No Borders."));
//            outputField->insertHtml(QString::fromStdString("<br />"));
//        }
//    }
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Public CustomTab Functions
/////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////
//void MeshCheckTab::onShapeDelete(Shape* shape) {
//    for(int i = comboBox->count()-1; i >= 0; i--) {
//        // check if items name matches the on in the combo box, if yes delete
//        if(comboBox->itemText(i).split(':')[0].toInt() == shape->getId()) {
//            // clear grid, if the deleted shape was the reference shape
//            if (i == comboBox->currentIndex()) {
//                outputField->clear();
//            }
//            comboBox->removeItem(i);
//            break;
//        }
//    }
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//void MeshCheckTab::onShapeAdd(Shape* shape) {
//    QString label = QString::number(shape->getId());
//    label.append(QString::fromStdString(":"+shape->getName()));
//    comboBox->addItem(label);
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//void MeshCheckTab::onShapeEdit(Shape* shape) {
//    QString label = QString::number(shape->getId());
//    label.append(QString::fromStdString(":"+shape->getName()));
//    
//    for(int i = comboBox->count()-1; i >= 0; i--) {
//        if(comboBox->itemText(i).split(':')[0].toInt() == shape->getId()) {
//            comboBox->setItemText(i, label);
//            break;
//        }
//    }
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//void MeshCheckTab::onClear() {
//    this->comboBox->clear();
//    this->outputField->clear();
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Private Functions
/////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////
//void MeshCheckTab::setUpComboBox() {
//    this->comboBox->clear();
//    
//    this->comboBox->insertItem(0, QString(tr(" ")));
//    
//    QStringList labels;
//    for(auto entry : shapes_) {
//        
//        QString label = QString::number(entry.second->getId());
//        label.append(QString::fromStdString(":"+entry.second->getName()));
//        labels << label;
//        
//    }
//    labels.sort();
//    this->comboBox->insertItems(1, labels);
//
//}

