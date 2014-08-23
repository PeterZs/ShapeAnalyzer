//
//  qtCorrespondencesTab.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#include "qtPointCorrespondencesTab.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
qtPointCorrespondencesTab::qtPointCorrespondencesTab(
                                                     HashMap<PointCorrespondenceData*, bool>* pointCorrespondences,
                                                     ShapeAnalyzer* parent,
                                                     Qt::WindowFlags f
                                                     )
:   QWidget(parent, f),
    pointCorrespondences_(pointCorrespondences),
    parent_(parent)
{
    this->setupUi(this);
    
    this->listPointCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);

    
    connect(this->buttonClear,                      SIGNAL(released()),
            this,                                   SLOT(slotClear()));
    
    connect(this->buttonSample,                     SIGNAL(released()),
            this,                                   SLOT(slotSample()));
    
    connect(this->listPointCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listPointCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
    
    // add all point correspondences to widget
    for(auto it = pointCorrespondences_->begin(); it != pointCorrespondences_->end(); it++) {
        string label = "Point Correspondence ";
        label.append(to_string(it->first->getId()+1));
        this->listPointCorrespondences->addItem(new qtListWidgetItem<PointCorrespondenceData>(
                                                QString(label.c_str()), it->first)
                                    );
    }
    
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::slotClear() {
    parent_->clearPointCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::slotOpenContextMenu(const QPoint& pos) {
    // current correspondence
    qtListWidgetItem<PointCorrespondenceData>* current =
    (qtListWidgetItem<PointCorrespondenceData>*) this->listPointCorrespondences->currentItem();
    
    
    QMenu menu;
    QAction* guiAction;
    if ((*pointCorrespondences_)[current->getItem()]) {
        guiAction = menu.addAction("Hide");
    } else {
        guiAction = menu.addAction("Show");
    }
    QAction* deleteAction = menu.addAction("Delete");
    
    
    QAction* selectedItem = menu.exec(this->listPointCorrespondences->mapToGlobal(pos));
    if (selectedItem == deleteAction) {
        parent_->deleteCorrespondence(current->getItem());
    } else if (selectedItem == guiAction) {
        if ((*pointCorrespondences_)[current->getItem()]){
            parent_->hideCorrespondence(current->getItem());
        } else {
            parent_->showCorrespondence(current->getItem());
        }
    }
}

void qtPointCorrespondencesTab::slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous) {
    parent_->setSelected(((qtListWidgetItem<PointCorrespondenceData>*) current)->getItem());
    
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::slotSample() {
    parent_->samplePointCorrespondences(this->spinBoxNumberOfDisplayedCorrespondences->value());
    if(listPointCorrespondences->currentRow() > -1) {
        parent_->setSelected(((qtListWidgetItem<PointCorrespondenceData>*) listPointCorrespondences->currentItem())->getItem());
    }
}

///////////////////////////////////////////////////////////////////////////////
// Events
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onClear() {
    listPointCorrespondences->disconnect();
    listPointCorrespondences->clear();
    connect(this->listPointCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listPointCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onCorrespondenceAdd(CorrespondenceData *correspondence) {
    if(correspondence->getType() == "PointCorrespondenceData") {
        string label = "Point Correspondence ";
        label.append(to_string(correspondence->getId()+1));
        this->listPointCorrespondences->addItem(new qtListWidgetItem<PointCorrespondenceData>(
                                                                                              QString(label.c_str()), (PointCorrespondenceData*) correspondence)
                                                );
    }
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onCorrespondenceDelete(CorrespondenceData* correspondence) {
    listPointCorrespondences->disconnect();

    for(int i = 0; i < listPointCorrespondences->count(); i++) {
        if(((qtListWidgetItem<PointCorrespondenceData>*) listPointCorrespondences->item(i))->getItem() == correspondence) {
            delete listPointCorrespondences->item(i);
            listPointCorrespondences->setCurrentRow(-1);
        }
    }
    connect(this->listPointCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listPointCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onCorrespondenceEdit(Correspondence* correspondence) {
    ; // Event thrown on "rename correspondence". Nothing to do here.
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onCorrespondenceSelect(Correspondence* correspondence) {
    //select item in list if clicked on shape
    for(int i = 0; i < listPointCorrespondences->count(); i++) {
        if(((qtListWidgetItem<PointCorrespondenceData>*) listPointCorrespondences->item(i))->getItem() == correspondence->getData()) {
            listPointCorrespondences->setCurrentRow(i);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onPointCorrespondencesClear() {
    listPointCorrespondences->disconnect();
    listPointCorrespondences->clear();
    connect(this->listPointCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    connect(this->listPointCorrespondences,         SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
}

///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::onFaceCorrespondencesClear() {
    ; // Nothing to do here.
}


