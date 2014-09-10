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
        if(it->second->getId() == comboBox->currentText().split(':')[0].toInt()) {
            shape = it->second;
            shapeFound = true;
            break;
        }
    }
    
    // return if no shape was found
    if(!shapeFound)
        return;
    
    // run all selected checks
    MeshChecker check = MeshChecker(shape);
    
    // triangulation check
    if (checkTriangles->isChecked()) {
        vector<pair<vtkIdType, vtkIdType> > nonTriangles;
        bool notTriangulated = check.checkTriangulation(&nonTriangles);
        
        if(notTriangulated) {
            outputField->insertPlainText(QString::fromStdString("- Non-Triangles in form (cell-id, #vertices): "));
            // list of all border edges
            for (auto it = nonTriangles.begin(); it != nonTriangles.end(); it++) {
                string tupel = "(";
                tupel.append(to_string(it->first));
                tupel.append(", ");
                tupel.append(to_string(it->second));
                tupel.append(") ");
                outputField->insertPlainText(QString::fromStdString(tupel));
            }
            outputField->insertHtml(QString::fromStdString("<br />"));
        } else {
            outputField->insertPlainText(QString::fromStdString("- All cells are triangles."));
            outputField->insertHtml(QString::fromStdString("<br />"));
        }
    }
    
    // number of regions
    if (checkConnectivity->isChecked()) {
        int numberRegions = check.checkNumberOfRegions();
        string regions = "- Number of connected Regions: ";
        regions.append(to_string(numberRegions));
        outputField->insertPlainText(QString::fromStdString(regions));
        outputField->insertHtml(QString::fromStdString("<br />"));
    }
    
    
    // orientation checking
    if (checkOrientation->isChecked()) {
        vector<pair<vtkIdType, vtkIdType> > unorientedEdges;
        bool isUnoriented = check.checkOrientation(&unorientedEdges);
        
        // output
        if(isUnoriented) {
            outputField->insertPlainText(QString::fromStdString("- Inconsistent orientation at: "));
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
            outputField->insertPlainText(QString::fromStdString("- Is consistently oriented."));
            outputField->insertHtml(QString::fromStdString("<br />"));
        }
    }
    
    
    // border checking
    if (checkBorders->isChecked()) {
        vector<pair<vtkIdType, vtkIdType> > borders;
        bool hasBorders = check.checkForBorders(&borders);
        
        // output
        if(hasBorders) {
            outputField->insertPlainText(QString::fromStdString("- Has Borders: "));
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
            outputField->insertPlainText(QString::fromStdString("- No Borders."));
            outputField->insertHtml(QString::fromStdString("<br />"));
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Public qtShapesTab Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onShapeDelete(Shape* shape) {
    for(int i = comboBox->count()-1; i >= 0; i--) {
        // check if items name matches the on in the combo box, if yes delete
        if(comboBox->itemText(i).split(':')[0].toInt() == shape->getId()) {
            // clear grid, if the deleted shape was the reference shape
            if (i == comboBox->currentIndex()) {
                outputField->clear();
            }
            comboBox->removeItem(i);
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onShapeAdd(Shape* shape) {
    QString label = QString::number(shape->getId());
    label.append(QString::fromStdString(":"+shape->getName()));
    comboBox->addItem(label);
}


///////////////////////////////////////////////////////////////////////////////
void qtMeshCheckTab::onShapeEdit(Shape* shape) {
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
/* Fills the ComboBox with the names of all shapes.
If an entry with the text of currentSelection is created, this entry will be 
selected. Otherwise the blank entry will be selected. */
void qtMeshCheckTab::setUpComboBox() {
    this->comboBox->clear();
    
    this->comboBox->insertItem(0, QString(tr(" ")));
    
    QStringList labels;
    for(HashMap<vtkActor*, Shape*>::iterator it = shapes_->begin(); it != shapes_->end(); it++) {
        
        QString label = QString::number(it->second->getId());
        label.append(QString::fromStdString(":"+it->second->getName()));
        labels << label;
        
    }
    labels.sort();
    this->comboBox->insertItems(1, labels);

}

