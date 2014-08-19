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
    for (int i = 0; i < shapes_->count(); i++) {
        this->comboBox->insertItem(i + 1, shapes_->item(i)->text());
    }
    
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
    for (int i = 0; i < shapes_->count(); i++) {
        if(shapes_->item(i)->text() == label) {
            referenceShape = ((qtListWidgetItem<Shape>*) shapes_->item(i))->getItem();
            referenceId = referenceShape->getId();
        }
    }
    
    // proceed if shape was found
    if (referenceId != -1) {
        CorrespondenceColoring cc = CorrespondenceColoring(map_,
                                                           pointCorr_,
                                                           faceCorr_,
                                                           referenceShape);
        if(pointRadio->isChecked()) {
            cc.showPointCorrespondences();
        } else {
            cc.showFaceCorrespondences();
        }

    }
}

