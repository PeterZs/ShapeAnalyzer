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
:   QWidget(parent, f),
    faceCorrespondences_(faceCorrespondences),
    parent_(parent)
{
    this->setupUi(this);
    
    this->listFaceCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);
    

    connect(this->buttonClear,                      SIGNAL(released()),
            this,                                   SLOT(slotClear()));
    
    connect(this->buttonSample,                     SIGNAL(released()),
            this,                                   SLOT(slotSample()));
    
    connect(this->listFaceCorrespondences,          SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listFaceCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                  SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
    

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
void qtFaceCorrespondencesTab::slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous) {
    parent_->setSelected(((qtListWidgetItem<FaceCorrespondenceData>*) current)->getItem());
    
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotSample() {
    parent_->sampleFaceCorrespondences(this->spinBoxNumberOfDisplayedCorrespondences->value());
    if(listFaceCorrespondences->currentRow() > -1) {
        parent_->setSelected(((qtListWidgetItem<FaceCorrespondenceData>*) listFaceCorrespondences->currentItem())->getItem());
    }
}

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onClear() {
    listFaceCorrespondences->disconnect();
    listFaceCorrespondences->clear();
    connect(this->listFaceCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listFaceCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onShapeDelete(Shape* shape) {
    listFaceCorrespondences->disconnect();
    
    for(int i = listFaceCorrespondences->count()-1; i > -1; i--) {
        FaceCorrespondenceData* data = ((qtListWidgetItem<FaceCorrespondenceData>*) listFaceCorrespondences->item(i))->getItem();
        
        for(int j = 0; j < data->getShapeIds().size(); j++) {
            if(data->getShapeIds()[j] == shape->getId()) {
                delete listFaceCorrespondences->item(i);
                break;
            }
        }
    }
    connect(this->listFaceCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                  SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listFaceCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                  SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onCorrespondenceAdd(CorrespondenceData *correspondence) {
    if(correspondence->getType() == "FaceCorrespondenceData") {
        string label = "Face Correspondence ";
        label.append(to_string(correspondence->getId()+1));
        this->listFaceCorrespondences->addItem(new qtListWidgetItem<FaceCorrespondenceData>(
                                                                                              QString(label.c_str()), (FaceCorrespondenceData*) correspondence)
                                                );
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onCorrespondenceDelete(CorrespondenceData* correspondence) {
    listFaceCorrespondences->disconnect();
    
    for(int i = listFaceCorrespondences->count()-1; i > -1; i--) {
        if(((qtListWidgetItem<FaceCorrespondenceData>*) listFaceCorrespondences->item(i))->getItem() == correspondence) {
            delete listFaceCorrespondences->item(i);
            listFaceCorrespondences->setCurrentRow(-1);
        }
    }
    connect(this->listFaceCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listFaceCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onCorrespondenceEdit(Correspondence* correspondence) {
    ; // Event thrown on "rename correspondence". Nothing to do here.
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onCorrespondenceSelect(Correspondence* correspondence) {
    //select item in list if clicked on shape
    for(int i = 0; i < listFaceCorrespondences->count(); i++) {
        if(((qtListWidgetItem<FaceCorrespondenceData>*) listFaceCorrespondences->item(i))->getItem() == correspondence->getData()) {
            listFaceCorrespondences->setCurrentRow(i);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onPointCorrespondencesClear() {
    ; // Nothing to do here
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::onFaceCorrespondencesClear() {
    listFaceCorrespondences->disconnect();
    listFaceCorrespondences->clear();
    connect(this->listFaceCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                  SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listFaceCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                  SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}

