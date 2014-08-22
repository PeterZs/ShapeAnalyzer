//
//  qtCorrespondenceTab.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#include "qtFaceCorrespondencesTab.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
qtFaceCorrespondencesTab::qtFaceCorrespondencesTab(
                                                HashMap<FaceCorrespondenceData*, bool>* faceCorrespondences,
                                                ShapeAnalyzer* parent,
                                                Qt::WindowFlags f
                                                )
:   QDialog(parent, f),
    faceCorrespondences_(faceCorrespondences),
    parent_(parent)
{
    this->setupUi(this);
    
    this->listFaceCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(this->listFaceCorrespondences,          SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    
    
    connect(this->buttonClear,                      SIGNAL(released()),
            this,                                   SLOT(slotClear()));
    
    connect(this->buttonSample,                     SIGNAL(released()),
            this,                                   SLOT(slotSample()));
    

    // add all face correspondences to widget
    for(auto it = faceCorrespondences_->begin(); it != faceCorrespondences_->end(); it++) {
        string label = "Face Correspondence ";
        label.append(to_string(it->first->getId()+1));
        this->listFaceCorrespondences->addItem(new qtListWidgetItem<FaceCorrespondenceData>(
                                                                                              QString(label.c_str()), it->first)
                                                );
    }
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotClear() {
    parent_->clearFaceCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotOpenContextMenu(const QPoint& pos) {
    // current correspondence
    qtListWidgetItem<FaceCorrespondenceData>* current =
    (qtListWidgetItem<FaceCorrespondenceData>*) this->listFaceCorrespondences->currentItem();
    
    
    QMenu menu;
    QAction* guiAction;
    if ((*faceCorrespondences_)[current->getItem()]) {
        guiAction = menu.addAction("Hide");
    } else {
        guiAction = menu.addAction("Show");
    }
    QAction* deleteAction = menu.addAction("Delete");
    
    
    QAction* selectedItem = menu.exec(this->listFaceCorrespondences->mapToGlobal(pos));
    if (selectedItem == deleteAction) {
        parent_->deleteCorrespondence(current->getItem());
    } else if (selectedItem == guiAction) {
        if ((*faceCorrespondences_)[current->getItem()]){
            parent_->hideCorrespondence(current->getItem());
        } else {
            parent_->showCorrespondence(current->getItem());
        }
    }
}




///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotSample() {
    parent_->sampleFaceCorrespondences(this->spinBoxNumberOfDisplayedCorrespondences->value());
}

