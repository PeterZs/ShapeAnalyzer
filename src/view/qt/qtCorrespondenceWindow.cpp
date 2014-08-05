//
//  qtCorrespondenceWindow.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#include "qtCorrespondenceWindow.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
qtCorrespondenceWindow::qtCorrespondenceWindow( Set<PointCorrespondenceData*, bool>* points,
                                                Set<FaceCorrespondenceData*, bool>* faces,
                                                Set<vtkActor*, PointCorrespondence*>* visiblePoints,
                                                Set<vtkActor*, FaceCorrespondence*>* visibleFaces,
                                                QListWidget* visibleList,
                                                QAction* pointMode,
                                                QWidget * parent,
                                                Qt::WindowFlags f
                                                )
:   QDialog(parent, f),
    pointCorr_(points),
    faceCorr_(faces),
    visiblePoints_(visiblePoints),
    visibleFaces_(visibleFaces),
    visibleList_(visibleList),
    pointMode_(pointMode)
{
    this->setupUi(this);
    
    this->listPoints->setContextMenuPolicy(Qt::CustomContextMenu);
    this->listFaces->setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(this->listFaces,                        SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenuFaces(const QPoint&)));
    
    connect(this->listPoints,                       SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenuPoints(const QPoint&)));
    
    connect(this->clearButton,                      SIGNAL(released()),
            this,                                   SLOT(slotClearCorrespondences()));
    
    // add all point correspondences to widget
    unsigned counter = 0;
    for(auto it = pointCorr_->begin(); it != pointCorr_->end(); it++) {
        string name = "Point Correspondence ";
        name.append(to_string(counter));
        this->listPoints->addItem(new qtListWidgetItem<PointCorrespondenceData>(
                                                QString(name.c_str()), it->first)
                                    );
        counter++;
    }
    
    // add all face correspondences to widget
    counter = 0;
    for(auto it = faceCorr_->begin(); it != faceCorr_->end(); it++) {
        string name = "Face Correspondence ";
        name.append(to_string(counter));
        this->listFaces->addItem(new qtListWidgetItem<FaceCorrespondenceData>(
                                                                                QString(name.c_str()), it->first)
                                  );
        counter++;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceWindow::slotClearCorrespondences() {
    pointCorr_->clear();
    faceCorr_->clear();
    
    for(auto it = visiblePoints_->begin(); it != visiblePoints_->end(); it++) {
        it->second->remove();
    }
    
    for(auto it = visibleFaces_->begin(); it != visibleFaces_->end(); it++) {
        it->second->remove();
    }
    
    visiblePoints_->clear();
    visibleFaces_->clear();
    visibleList_->clear();
    
    this->listPoints->clear();
    this->listFaces->clear();
    
    ShapeAnalyzer* parent = (ShapeAnalyzer*) this->parent();
    parent->render();
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceWindow::slotOpenContextMenuFaces(const QPoint& pos) {
    // current correspondence
    qtListWidgetItem<FaceCorrespondenceData>* current =
    (qtListWidgetItem<FaceCorrespondenceData>*) this->listFaces->currentItem();
    
    
    QMenu menu;
    QAction* guiAction;
    if (this->faceCorr_->getValue(current->getItem())){
        guiAction   = menu.addAction("Hide");
    } else {
        guiAction   = menu.addAction("Show");
    }
    QAction* deleteAction   = menu.addAction("Delete");
    
    // TODO this is not opening at the right position
    QAction* selectedItem = menu.exec(pos);
    //if (selectedItem == deleteAction) {
    //    deleteCorrespondence(this->listCorrespondences->currentRow());
    //} else if (selectedItem == renameAction) {
    //    qtInputDialogRename(this->listCorrespondences->currentItem());
    //}
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceWindow::slotOpenContextMenuPoints(const QPoint& pos) {
    // current correspondence
    qtListWidgetItem<PointCorrespondenceData>* current =
    dynamic_cast<qtListWidgetItem<PointCorrespondenceData>* > (this->listPoints->currentItem());
    
    
    QMenu menu;
    QAction* guiAction;
    if (this->pointCorr_->getValue(current->getItem())){
        guiAction   = menu.addAction("Hide");
    } else {
        guiAction   = menu.addAction("Show");
    }
    QAction* deleteAction   = menu.addAction("Delete");
    
    // TODO this is not opening at the right position
    QAction* selectedItem = menu.exec(pos);
    //if (selectedItem == deleteAction) {
    //    deleteCorrespondence(this->listCorrespondences->currentRow());
    //} else if (selectedItem == renameAction) {
    //    qtInputDialogRename(this->listCorrespondences->currentItem());
    //}
}


///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceWindow::slotRandomSubsetFaces() {
    if(pointMode_->isChecked()) {
        ShapeAnalyzer* parent = (ShapeAnalyzer*) parentWidget();
        parent->switchCorrespondenceMode();
    }
    
    visibleList_->clear();
    visibleFaces_->clear();
    
    unsigned size = this->spinBoxFaceNumber->value();
    
    
}

///////////////////////////////////////////////////////////////////////////////
void qtCorrespondenceWindow::slotRandomSubsetPoints() {
    if(pointMode_->isChecked()) {
        ShapeAnalyzer* parent = (ShapeAnalyzer*) parentWidget();
        parent->switchCorrespondenceMode();
    }
    
    visibleList_->clear();
    visibleFaces_->clear();
    
    unsigned size = this->spinBoxFaceNumber->value();
    
    
}

