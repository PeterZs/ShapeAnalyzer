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
                                                     HashMap<vtkActor*, PointCorrespondence*>* visiblePointCorrespondences,
                                                     HashMap<vtkActor*, Shape*>* shapes,
                                                     QListWidget* visiblePointCorrespondencesList,
                                                     QAction* pointMode,
                                                     vtkSmartPointer<vtkRenderer> renderer,
                                                     QWidget * parent,
                                                     Qt::WindowFlags f
                                                     )
:   QWidget(parent, f),
    pointCorrespondences_(pointCorrespondences),
    visiblePointCorrespondences_(visiblePointCorrespondences),
    shapes_(shapes),
    visiblePointCorrespondencesList_(visiblePointCorrespondencesList),
    pointMode_(pointMode),
    renderer_(renderer)
{
    this->setupUi(this);
    
    this->listPointCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);

    
    connect(this->listPointCorrespondences,         SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotOpenContextMenu(const QPoint&)));
    
    connect(this->buttonClear,                      SIGNAL(released()),
            this,                                   SLOT(slotClear()));
    
    // add all point correspondences to widget
    unsigned counter = 0;
    for(auto it = pointCorrespondences_->begin(); it != pointCorrespondences_->end(); it++) {
        string name = "Point Correspondence ";
        name.append(to_string(counter));
        this->listPointCorrespondences->addItem(new qtListWidgetItem<PointCorrespondenceData>(
                                                QString(name.c_str()), it->first)
                                    );
        counter++;
    }
    
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::slotClear() {
    pointCorrespondences_->clear();
    
    for(auto it = visiblePointCorrespondences_->begin(); it != visiblePointCorrespondences_->end(); it++) {
        it->second->remove();
    }
    

    
    visiblePointCorrespondences_->clear();
    visiblePointCorrespondencesList_->clear();
    
    this->listPointCorrespondences->clear();
    
    ShapeAnalyzer* parent = (ShapeAnalyzer*) this->parent();
    parent->render();
}


///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::slotOpenContextMenu(const QPoint& pos) {
    // current correspondence
    qtListWidgetItem<PointCorrespondenceData>* current =
    (qtListWidgetItem<PointCorrespondenceData>*) this->listPointCorrespondences->currentItem();
    
    
    QMenu menu;
    QAction* guiAction;
    if ((*pointCorrespondences_)[current->getItem()]) {
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
        if ((*pointCorrespondences_)[current->getItem()]){
            guiAction   = menu.addAction("Hide");
        } else {
            addPointCorrespondence(current->getItem());
        }
    }
}




///////////////////////////////////////////////////////////////////////////////
void qtPointCorrespondencesTab::slotRandomSubset() {
    if(pointMode_->isChecked()) {
        ShapeAnalyzer* parent = (ShapeAnalyzer*) parentWidget();
        parent->switchCorrespondenceMode();
    }
    
    visiblePointCorrespondencesList_->clear();
    visiblePointCorrespondences_->clear();
    
    //unsigned size = this->spinBoxFaceNumber->value();
    
    
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

void qtPointCorrespondencesTab::addPointCorrespondence(PointCorrespondenceData* data) {
    if(!pointMode_->isChecked()) {
        ShapeAnalyzer* parent = (ShapeAnalyzer*) parentWidget();
        parent->switchCorrespondenceMode();
    }
    PointCorrespondence* corr = new PointCorrespondence(renderer_, data, shapes_);
    // create actor and add to vtk
    corr->produceActorFromData();
    corr->add();
    visiblePointCorrespondences_->add(corr->getLinesActor(), corr);
}
