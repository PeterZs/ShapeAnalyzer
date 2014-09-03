//
//  qtMeshCheckTab.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 26.08.14.
//
//

#include "qtMeshCheckTab.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
qtMeshCheckTab::qtMeshCheckTab( HashMap<vtkActor*, Shape*>* shapes,
                                QWidget *                   parent,
                                Qt::WindowFlags             f
                              )
:   QWidget(parent, f), shapes_(shapes)
{
    this->setupUi(this);
    
    setUpComboBox();
    
    connect(this->buttonCheckMesh,  SIGNAL(released()),
            this,                   SLOT(slotCheckMesh()));
    connect(this->comboBox,         SIGNAL(currentIndexChanged(int)),
            this->outputField,      SLOT(clear()));
}


///////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::slotCheckMesh() {
    // clear previous output
    outputField->clear();
    
    // find correct shape
    Shape* shape;
    bool shapeFound = false;
    for (auto it = shapes_->begin(); it != shapes_->end(); it++) {
        if(comboBox->currentText().toStdString() == it->second->getName()) {
            shape = it->second;
            shapeFound = true;
        }
    }
    
    // return if no shape was found
    if(!shapeFound)
        return;
    
    // run all selected checks
    MeshChecker check = MeshChecker(shape);
    
    
    // orientation checking
    if (checkOrientation->isChecked()) {
        vector<pair<vtkIdType, vtkIdType> > unorientedEdges;
        bool isUnoriented = check.checkOrientation(&unorientedEdges);
        
        // output
        if(isUnoriented) {
            outputField->insertPlainText(QString::fromStdString("Inconsistent orientation at: "));
            // list of all border edges
            for (auto it = unorientedEdges.begin(); it != unorientedEdges.end(); it++) {
                string tupel = "(";
                tupel.append(to_string(it->first));
                tupel.append(", ");
                tupel.append(to_string(it->second));
                tupel.append(") ");
                outputField->insertPlainText(QString::fromStdString(tupel));
            }
            outputField->insertHtml(QString::fromStdString("<br />"));
        } else {
            outputField->insertPlainText(QString::fromStdString("Is consistently oriented."));
            outputField->insertHtml(QString::fromStdString("<br />"));
        }
    }
    
    
    // border checking
    if (checkBorders->isChecked()) {
        vector<pair<vtkIdType, vtkIdType> > borders;
        bool hasBorders = check.checkForBorders(&borders);
        
        // output
        if(hasBorders) {
            outputField->insertPlainText(QString::fromStdString("Has Borders: "));
            // list of all border edges
            for (auto it = borders.begin(); it != borders.end(); it++) {
                string tupel = "(";
                tupel.append(to_string(it->first));
                tupel.append(", ");
                tupel.append(to_string(it->second));
                tupel.append(") ");
                outputField->insertPlainText(QString::fromStdString(tupel));
            }
            outputField->insertHtml(QString::fromStdString("<br />"));
        } else {
            outputField->insertPlainText(QString::fromStdString("No Borders."));
            outputField->insertHtml(QString::fromStdString("<br />"));
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Public qtShapesTab Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// TODO
void qtMeshCheckTab::onShapeDelete(Shape* shape) {
    if (shape->getName() == comboBox->currentText().toStdString()) {
        outputField->clear();
    }
    setUpComboBox(comboBox->currentText());
}


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onShapeAdd(Shape* shape) {
    comboBox->addItem(QString::fromStdString(shape->getName()));
}


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onShapeEdit(Shape* shape) {
    setUpComboBox(comboBox->currentText());
}


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onShapeSelect(Shape* shape) {
    
}


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onClear() {
    this->comboBox->clear();
    this->outputField->clear();
}


///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Fills the ComboBox with the names of all shapes.
// If an entry with the text of currentSelection is created, this entry will be
// selected. Otherwise the blank entry will be selected.
void qtMeshCheckTab::setUpComboBox(QString currentSelection) {
    this->comboBox->clear();
    
    // create combo selection for shapes
    this->comboBox->insertItem(0, QString(tr(" ")));
    int currentIndex = 0;
    int i = 0;
    
    // add all shapes to combo box
    for (auto it = shapes_->begin(); it != shapes_->end(); it++) {
        this->comboBox->insertItem(i + 1, QString::fromStdString(it->second->getName()));
        i++;
        // remember index of given selection if required
        if (QString::fromStdString(it->second->getName()) == currentSelection) {
            currentIndex = i;
        }
    }
    
    comboBox->setCurrentIndex(currentIndex);

}

