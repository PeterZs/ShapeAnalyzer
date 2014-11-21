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
    
    this->comboBox->insertItem(0, QString(tr(" ")));
    
    QStringList labels;
    for(auto entry : shapes_) {
        
        QString label = QString::number(entry.second->getId());
        label.append(QString::fromStdString(":"+entry.second->getName()));
        labels << label;
        
    }
    labels.sort();
    this->comboBox->insertItems(1, labels);
    
    connect(this->comboBox,         SIGNAL(currentIndexChanged(QString)),
            this,                   SLOT(slotColorCorrespondences(QString)));
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::slotColorCorrespondences(const QString label) {
    vtkIdType referenceId = -1;
    Shape* referenceShape;
    
    // get Shape corresponding with the label
    for (auto entry : shapes_) {
        if(entry.second->getId() == label.split(':')[0].toInt()) {
            referenceShape = entry.second;
            referenceId = referenceShape->getId();
        }
    }
    
    // proceed if shape was found
    if (referenceId != -1) {
        CorrespondenceColoring cc = CorrespondenceColoring(shapes_,
                                                           pointCorrespondences_,
                                                           faceCorrespondences_,
                                                           referenceShape);
        
        vector<pair<vtkIdType, double> > matched = vector<pair<vtkIdType, double> >();
        vector<pair<vtkIdType, double> > multiple = vector<pair<vtkIdType, double> >();
        
        if(pointRadio->isChecked()) {
            cc.showPointCorrespondences(&matched, &multiple);
            
            clearGrid();
            
            // fill in percentage of matched points
            int index = 1;
            for (auto entry : shapes_) {
                if (entry.second->getId() != referenceId) {
                    // shape name
                    QString label = QString::fromStdString(entry.second->getName());
                    gridLayout->addWidget(new QLabel(label), index, 0);
                    
                    // % matched
                    for (int i = 0; i < matched.size(); i++) {
                        if (matched[i].first == entry.second->getId()) {
                            QString labelMatched = QString::fromStdString(std::to_string(matched[i].second));
                            gridLayout->addWidget(new QLabel(labelMatched), index, 1);
                            break;
                        }
                    }
                    
                    // % multiple
                    for (int i = 0; i < multiple.size(); i++) {
                        if (multiple[i].first == entry.second->getId()) {
                            QString labelMultiple = QString::fromStdString(std::to_string(multiple[i].second));
                            gridLayout->addWidget(new QLabel(labelMultiple), index, 2);
                            break;
                        }
                    }
                    
                    index++;
                }
            }
            
        } else {
            cc.showFaceCorrespondences(&matched, &multiple);
            
            clearGrid();
            
            // fill in percentage of matched points
            int index = 1;
            for (auto entry : shapes_) {
                if (entry.second->getId() != referenceId) {
                    // shape name
                    QString label = QString::fromStdString(entry.second->getName());
                    gridLayout->addWidget(new QLabel(label), index, 0);
                    
                    // % matched
                    for (int i = 0; i < matched.size(); i++) {
                        if (matched[i].first == entry.second->getId()) {
                            QString labelMatched = QString::fromStdString(std::to_string(matched[i].second));
                            gridLayout->addWidget(new QLabel(labelMatched), index, 1);
                            break;
                        }
                    }
                    
                    // % multiple
                    for (int i = 0; i < multiple.size(); i++) {
                        if (multiple[i].first == entry.second->getId()) {
                            QString labelMultiple = QString::fromStdString(std::to_string(multiple[i].second));
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
// qtShapeTab Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onShapeDelete(Shape *shape) {
    for(int i = comboBox->count()-1; i >= 0; i--) {
        // check if items name matches the on in the combo box, if yes delete
        if(comboBox->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBox->removeItem(i);
            // clear grid, if the deleted shape was the reference shape
            if (i == comboBox->currentIndex()) {
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
    comboBox->addItem(label);
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onShapeEdit(Shape *shape) {
    // nothing happens
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::onClear() {
    comboBox->clear();
    clearGrid();
}


///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoringTab::clearGrid() {
    for (int i = 1; i < gridLayout->rowCount(); i++) {
        for (int j = 0; j < gridLayout->columnCount(); j++) {
            QWidget* item = gridLayout->itemAtPosition(i, j)->widget();
            delete item;
        }
    }
}
