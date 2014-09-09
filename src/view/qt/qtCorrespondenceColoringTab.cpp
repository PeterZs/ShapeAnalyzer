//
//  qtCorrespondenceColoringTab.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 04.08.14.
//
//

#include "qtCorrespondenceColoringTab.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
qtCorrespondenceColoringTab::qtCorrespondenceColoringTab( QListWidget* shapes,
                                                         HashMap<vtkActor*, Shape*>*             map,
                                                         HashMap<FaceCorrespondenceData*, bool>* faceCorr,
                                                         HashMap<PointCorrespondenceData*, bool>* pointCorr,
                                                         QWidget * parent,
                                                         Qt::WindowFlags f
                                                         )
:   QWidget(parent, f),
shapes_(shapes),
map_(map),
faceCorr_(faceCorr),
pointCorr_(pointCorr)
{
    this->setupUi(this);
    
    this->comboBox->insertItem(0, QString(tr(" ")));
    
    QStringList labels;
    for(HashMap<vtkActor*, Shape*>::iterator it = map_->begin(); it != map_->end(); it++) {
        
        QString label = QString::number(it->second->getId());
        label.append(QString::fromStdString(":"+it->second->getName()));
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
void qtCorrespondenceColoringTab::slotColorCorrespondences(const QString label) {
    vtkIdType referenceId = -1;
    Shape* referenceShape;
    
    // get Shape corresponding with the label
    for (auto it = map_->begin(); it != map_->end(); it++) {
        if(it->second->getId() == label.split(':')[0].toInt()) {
            referenceShape = it->second;
            referenceId = referenceShape->getId();
        }
    }
    
    // proceed if shape was found
    if (referenceId != -1) {
        CorrespondenceColoring cc = CorrespondenceColoring(map_,
                                                           pointCorr_,
                                                           faceCorr_,
                                                           referenceShape);
        
        vector<pair<vtkIdType, double> > matched = vector<pair<vtkIdType, double> >();
        vector<pair<vtkIdType, double> > multiple = vector<pair<vtkIdType, double> >();
        
        if(pointRadio->isChecked()) {
            cc.showPointCorrespondences(&matched, &multiple);
            
            // fill in percentage of matched points
            int index = 1;
            for (auto it = map_->begin(); it != map_->end(); it++) {
                if (it->second->getId() != referenceId) {
                    // shape name
                    QString label = QString::fromStdString(it->second->getName());
                    gridLayout->addWidget(new QLabel(label), index, 0);
                    
                    // % matched
                    for (int i = 0; i < matched.size(); i++) {
                        if (matched[i].first == it->second->getId()) {
                            QString labelMatched = QString::fromStdString(std::to_string(matched[i].second));
                            gridLayout->addWidget(new QLabel(labelMatched), index, 1);
                            break;
                        }
                    }
                    
                    // % multiple
                    for (int i = 0; i < multiple.size(); i++) {
                        if (multiple[i].first == it->second->getId()) {
                            QString labelMultiple = QString::fromStdString(std::to_string(multiple[i].second));
                            gridLayout->addWidget(new QLabel(labelMultiple), index, 2);
                            break;
                        }
                    }
                    
                    index++;
                }
            }
            
        } else {
            cc.showFaceCorrespondences();
        }

    }
}


///////////////////////////////////////////////////////////////////////////////
// qtShapeTab Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceColoringTab::onShapeDelete(Shape *shape) {
    for(int i = comboBox->count()-1; i >= 0; i--) {
        if(comboBox->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBox->removeItem(i);
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceColoringTab::onShapeAdd(Shape *shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBox->addItem(label);
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceColoringTab::onShapeEdit(Shape *shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    
    for(int i = comboBox->count()-1; i >= 0; i--) {
        if(comboBox->itemText(i).split(':')[0].toInt() == shape->getId()) {
            comboBox->setItemText(i, label);
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceColoringTab::onShapeSelect(Shape *shape) {
    // nothing
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceColoringTab::onClear() {
    comboBox->clear();
}
