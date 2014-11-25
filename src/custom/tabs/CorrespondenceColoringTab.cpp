//
//  CorrespondenceColoringTab.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

#include "CorrespondenceColoringTab.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
CorrespondenceColoringTab::CorrespondenceColoringTab(
                                                     const HashMap<vtkActor*, Shape*>& shapes,
                                                     const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                                                     const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent)
: QWidget(parent, 0), CustomTab(shapes, pointCorrespondences, faceCorrespondences, parent)
{
    this->setupUi(this);
    

    QStringList labels;
    for(auto entry : shapes_) {
        
        QString label = QString::number(entry.second->getId());
        label.append(QString::fromStdString(":"+entry.second->getName()));
        labels << label;
        
    }
    labels.sort();
    this->comboBoxReference->insertItems(0, labels);
    
    connect(this->buttonColor,         SIGNAL(released()),
            this,                      SLOT(slotColorCorrespondences()));
    
    connect(this->buttonClearColoring, SIGNAL(released()),
            this,                      SLOT(slotClearColoring()));
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::slotColorCorrespondences() {
    Shape* reference = nullptr;
    
    // get Shape corresponding with the label
    for (auto entry : shapes_) {
        if(entry.second->getId() == comboBoxReference->currentText().split(':')[0].toInt()) {
            reference = entry.second;
            break;
        }
    }
    
    // proceed if shape was found
    if (reference != nullptr) {
        CorrespondenceColoring cc = CorrespondenceColoring(shapes_,
                                                           pointCorrespondences_,
                                                           faceCorrespondences_,
                                                           reference);
        
        shared_ptr<vector<pair<vtkIdType, double>>> matched = make_shared<vector<pair<vtkIdType, double>>>();
        shared_ptr<vector<pair<vtkIdType, double>>> multiple = make_shared<vector<pair<vtkIdType, double>>>();
        
        if(radioButtonPointCorrespondences->isChecked()) {
            cc.showPointCorrespondences(matched, multiple);
            
            clearGrid();
            
            // fill in percentage of matched points
            int index = 1;
            for (auto entry : shapes_) {
                if (entry.second != reference) {
                    // shape name
                    QString label = QString::fromStdString(entry.second->getName());
                    gridLayout->addWidget(new QLabel(label), index, 0);
                    
                    // % matched
                    for (int i = 0; i < matched->size(); i++) {
                        if (matched->at(i).first == entry.second->getId()) {
                            QString labelMatched = QString::fromStdString(std::to_string(matched->at(i).second));
                            gridLayout->addWidget(new QLabel(labelMatched), index, 1);
                            break;
                        }
                    }
                    
                    // % multiple
                    for (int i = 0; i < multiple->size(); i++) {
                        if (multiple->at(i).first == entry.second->getId()) {
                            QString labelMultiple = QString::fromStdString(std::to_string(multiple->at(i).second));
                            gridLayout->addWidget(new QLabel(labelMultiple), index, 2);
                            break;
                        }
                    }
                    
                    index++;
                }
            }
            
        } else {
            cc.showFaceCorrespondences(matched, multiple);
            
            clearGrid();
            
            // fill in percentage of matched points
            int index = 1;
            for (auto entry : shapes_) {
                if (entry.second != reference) {
                    // shape name
                    QString label = QString::fromStdString(entry.second->getName());
                    gridLayout->addWidget(new QLabel(label), index, 0);
                    
                    // % matched
                    for (int i = 0; i < matched->size(); i++) {
                        if (matched->at(i).first == entry.second->getId()) {
                            QString labelMatched = QString::fromStdString(std::to_string(matched->at(i).second));
                            gridLayout->addWidget(new QLabel(labelMatched), index, 1);
                            break;
                        }
                    }
                    
                    // % multiple
                    for (int i = 0; i < multiple->size(); i++) {
                        if (multiple->at(i).first == entry.second->getId()) {
                            QString labelMultiple = QString::fromStdString(std::to_string(multiple->at(i).second));
                            gridLayout->addWidget(new QLabel(labelMultiple), index, 2);
                            break;
                        }
                    }
                    
                    index++;
                }
            }
        }

    }
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::slotClearColoring() {
    for (auto entry : shapes_) {
        entry.second->clearColoring();
    }
    clearGrid();
}


///////////////////////////////////////////////////////////////////////////////
// Public CustomTab Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onShapeDelete(Shape *shape) {
    for(int i = comboBoxReference->count()-1; i >= 0; i--) {
        // check if items name matches the on in the combo box, if yes delete
        if(comboBoxReference->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxReference->removeItem(i);
            // clear grid, if the deleted shape was the reference shape
            if (i == comboBoxReference->currentIndex()) {
                clearGrid();
            }
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onShapeAdd(Shape *shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBoxReference->addItem(label);
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onShapeEdit(Shape *shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    
    for(int i = comboBoxReference->count()-1; i >= 0; i--) {
        if(comboBoxReference->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBoxReference->setItemText(i, label);
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onClear() {
    comboBoxReference->clear();
    clearGrid();
}


///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::clearGrid() {
    for (int i = 1; i < gridLayout->rowCount(); i++) {
        for (int j = 0; j < gridLayout->columnCount(); j++) {
            if(gridLayout->itemAtPosition(i, j) != nullptr)
                delete gridLayout->itemAtPosition(i, j)->widget();
        }
    }
}
