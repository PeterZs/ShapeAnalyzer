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
                                                HashMap<vtkActor*, FaceCorrespondence*>* visibleFaceCorrespondences,
                                                HashMap<vtkActor*, Shape*>* shapes,
                                                QListWidget* visibleFaceCorrespondencesList,
                                                QAction* pointMode,
                                                vtkSmartPointer<vtkRenderer> renderer,
                                                QWidget * parent,
                                                Qt::WindowFlags f
                                                )
:   QDialog(parent, f),
    faceCorrespondences_(faceCorrespondences),
    visibleFaceCorrespondences_(visibleFaceCorrespondences),
    shapes_(shapes),
    visibleFaceCorrespondencesList_(visibleFaceCorrespondencesList),
    pointMode_(pointMode),
    renderer_(renderer)
{
    this->setupUi(this);
    
    this->listFaceCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(this->listFaceCorrespondences,          SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    
    
    connect(this->buttonClear,                      SIGNAL(released()),
            this,                                   SLOT(slotClear()));
    
    // add all face correspondences to widget
    unsigned counter = 0;

    

    for(auto it = faceCorrespondences_->begin(); it != faceCorrespondences_->end(); it++) {
        string name = "Face Correspondence ";
        name.append(to_string(counter));
        this->listFaceCorrespondences->addItem(new qtListWidgetItem<FaceCorrespondenceData>(
                                                                                QString(name.c_str()), it->first)
                                  );
        counter++;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotClear() {
    faceCorrespondences_->clear();
    

    
    for(auto it = visibleFaceCorrespondences_->begin(); it != visibleFaceCorrespondences_->end(); it++) {
        it->second->remove();
    }
    
    visibleFaceCorrespondences_->clear();
    visibleFaceCorrespondencesList_->clear();
    
    this->listFaceCorrespondences->clear();
    
    ShapeAnalyzer* parent = (ShapeAnalyzer*) this->parent();
    parent->render();
}


///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotOpenContextMenu(const QPoint& pos) {
    // current correspondence
    qtListWidgetItem<FaceCorrespondenceData>* current =
    (qtListWidgetItem<FaceCorrespondenceData>*) this->listFaceCorrespondences->currentItem();
    
    
    QMenu menu;
    QAction* guiAction;

    
    if ((*faceCorrespondences_)[current->getItem()]) {
        guiAction   = menu.addAction("Hide");
    } else {
        guiAction   = menu.addAction("Show");
    }
    QAction* deleteAction   = menu.addAction("Delete");
    
    // TODO this is not opening at the right position
    QAction* selectedItem = menu.exec(pos);
    //if (selectedItem == deleteAction) {
    //    deleteCorrespondence(this->listCorrespondences->currentRow());
    //} else
    if (selectedItem == guiAction) {
        
    }
}




///////////////////////////////////////////////////////////////////////////////
void qtFaceCorrespondencesTab::slotRandomSubset() {
    if(pointMode_->isChecked()) {
        ShapeAnalyzer* parent = (ShapeAnalyzer*) parentWidget();
        parent->switchCorrespondenceMode();
    }
    
    visibleFaceCorrespondencesList_->clear();
    visibleFaceCorrespondences_->clear();
    
    unsigned size = this->spinBoxNumberOfDisplayedCorrespondences->value();
    
    
}

