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
                                                         Set<vtkActor*, Shape*>*              set,
                                                         Set<FaceCorrespondenceData*, bool>* faces,
                                                         Set<PointCorrespondenceData*, bool>* points,
                                                         QWidget * parent,
                                                         Qt::WindowFlags f
                                                         )
:   QWidget(parent, f),
shapes_(shapes),
set_(set),
faces_(faces),
points_(points)
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
        
        CorrespondenceColoring cc = CorrespondenceColoring(set_,
                                                           points_,
                                                           faces_,
                                                           referenceShape);
        cc.showPointCorrespondences();

    }
}

