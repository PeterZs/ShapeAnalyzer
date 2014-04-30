#include "ShapeAnalyzer.h"

#include <map>
// Constructor
ShapeAnalyzer::ShapeAnalyzer() {
    this->setupUi(this);

    this->listShapes->setContextMenuPolicy(Qt::CustomContextMenu);
    this->listCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);

    // Set up action signals and slots
    connect(this->actionExit,                       SIGNAL(triggered()),
            this,                                   SLOT(slotExit()));
    connect(this->actionResetCamera,                SIGNAL(triggered()),
            this,                                   SLOT(slotResetCamera()));
    connect(this->actionClear,                      SIGNAL(triggered()),
            this,                                   SLOT(slotClear()));
    connect(this->clearButton,                      SIGNAL(clicked()),
            this,                                   SLOT(slotClear()));
    connect(this->openFileButton,                   SIGNAL(clicked()),
            this,                                   SLOT(slotOpenShape()));
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleBoxWidget()));
    connect(this->radioButtonAddCorrespondences,    SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleBoxWidget()));
    connect(this->radioButtonTransformScene,        SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleBoxWidget()));
    
    //delete selected correspondence triangle if mode was changed. This triggers box widget to show up on shape if shape has been selected.
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(slotClearCurrentSelection()));
    connect(this->radioButtonAddCorrespondences,       SIGNAL(toggled(bool)),
            this,                                   SLOT(slotClearCurrentSelection()));
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(slotClearCurrentSelection()));
    
    connect(this->actionHelp,                       SIGNAL(triggered()),
            this,                                   SLOT(slotOpenHelpWindow()));
    
    //connection of list widgets is done in extra functions since signals of list widgets are disconnected before and reconnected after deletion of list items
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
    this->vtkSetup();
}


void ShapeAnalyzer::qtConnectListCorrespondences() {
    listCorrespondences->setCurrentRow(-1);
    connect(this->listCorrespondences,              SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetCurrentCorrespondenceColor(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listCorrespondences,              SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotShowContextMenuCorrespondences(const QPoint&)));

    
    //manually call slot set box widget
    slotSetCurrentCorrespondenceColor(listCorrespondences->currentItem(), nullptr);
}

void ShapeAnalyzer::qtConnectListShapes() {
    connect(this->listShapes,                       SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetCurrentBoxWidget(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listShapes,                       SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotShowContextMenuShapes(const QPoint&)));

    slotSetCurrentBoxWidget(listShapes->currentItem(), nullptr);
}

///////////////////////////////////////////////////////////////////////////////
// Functions handling QT 
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//catch mouse wheel events since they cause strange behavior on Mac OSX
bool ShapeAnalyzer::eventFilter(QObject *object, QEvent *event) {
    if (object == qvtkWidget && event->type() == QEvent::Wheel) {
        return true;
    }
    
    return false;
}

void ShapeAnalyzer::qtShowContextMenuCorrepondences(const QPoint &pos) {
    QMenu menu;
    menu.addAction("Delete");
    // ...
    
    QAction* selectedItem = menu.exec(pos);
    if (selectedItem) {
        deleteCorrespondence(this->listCorrespondences->currentRow());
    }
}

void ShapeAnalyzer::qtShowContextMenuShapes(const QPoint &pos) {
    QMenu menu;
    menu.addAction("Delete");
    // ...
    
    QAction* selectedItem = menu.exec(pos);
    if (selectedItem) {
        deleteShape(this->listShapes->currentRow());
    }
}

///////////////////////////////////////////////////////////////////////////////
// QT slots
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotExit() {
    qApp->exit();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotClearCurrentSelection() {
    correspondencePicker_->clearSelection();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotOpenHelpWindow() {
    
    QDialog* help = new QDialog(0,0);

    Ui_Dialog helpUi;
    helpUi.setupUi(help);

    help->show();

}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotOpenShape() {
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.off)"));
    
    if(fileName.count() == 0)
        return;
    
    Shape* shape = vtkAddShape(fileName);

    lastInsertShapeID_++;

    // add shape to qt list widget
    string name = "Shape ";
    name.append(std::to_string(lastInsertShapeID_));
    
    ShapeListItem *item = new ShapeListItem(QString(name.c_str()), shape);
    listShapes->addItem(item);
    
    //make sure that there always is exactly one item selected if there exists at least one item
    listShapes->setCurrentRow(listShapes->count()-1);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotShowContextMenuCorrespondences(const QPoint& pos) {
    int currentRow = this->listCorrespondences->currentRow();

    // menu appears only if item was selected
    if(currentRow >= 0) {

        QPoint globalPos = this->listCorrespondences->mapToGlobal(pos);
        // for QAbstractScrollArea and derived classes you would use:
        // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

        qtShowContextMenuCorrepondences(globalPos);
    }
}


///////////////////////////////////////////////////////////////////////////////
// opens menu for shapes
void ShapeAnalyzer::slotShowContextMenuShapes(const QPoint& pos) {
    int currentRow = this->listShapes->currentRow();

    // menu appears only if item was selected
    if(currentRow >= 0) {

        QPoint globalPos = this->listShapes->mapToGlobal(pos);
        // for QAbstractScrollArea and derived classes you would use:
        // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

        qtShowContextMenuShapes(globalPos);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotToggleBoxWidget() {
    if(listShapes->count() > 0) {
        Shape* selectedShape = dynamic_cast<ShapeListItem*>(listShapes->currentItem())->getShape();
        if(this->radioButtonTransformActors->isChecked()) {
            selectedShape->getBoxWidget()->On();
        } else {
            selectedShape->getBoxWidget()->Off();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous) {
    if(this->radioButtonTransformActors->isChecked()) {
        if(current != nullptr) {
            dynamic_cast<ShapeListItem*>(current)->getShape()->getBoxWidget()->On();
            
            if(previous != nullptr) {
                dynamic_cast<ShapeListItem*>(previous)->getShape()->getBoxWidget()->Off();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotClear() {
    clear();
}

///////////////////////////////////////////////////////////////////////////////
// Slots handling VTK
///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotResetCamera() {
    renderer_->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetCurrentCorrespondenceColor(QListWidgetItem* current, QListWidgetItem* previous) {
    // only change color if there is a selection
    if(current != nullptr) {
        // set current correspondence red
        dynamic_cast<CorrespondenceListItem*>(current)->getCorrespondence()->getActor()->GetProperty()->SetColor(1, 0, 0);
        
        //if there exists a previous selection set previous correspondence to green
        if(previous != nullptr) {
            dynamic_cast<CorrespondenceListItem*>(previous)->getCorrespondence()->getActor()->GetProperty()->SetColor(0, 1, 0);
        }
        
        // update
        this->qvtkWidget->GetRenderWindow()->Render();
    }
}


///////////////////////////////////////////////////////////////////////////////
// Functions handling VTK
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
Shape* ShapeAnalyzer::vtkAddShape(QString fileName) {
    Shape* shape = new Shape;

    // read .off file
    vtkSmartPointer<vtkOFFReader> reader = vtkSmartPointer<vtkOFFReader>::New();
    reader->SetFileName(fileName.toUtf8().constData());

    //make shure that shape consists of triangles
    vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    triangleFilter->SetInputConnection(reader->GetOutputPort());
    triangleFilter->Update();
    shape->setData(triangleFilter->GetOutput());
    
    // get vtk actor and add to renderer_
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(triangleFilter->GetOutputPort());

    shape->setActor(vtkSmartPointer<vtkActor>::New());
    shape->getActor()->SetMapper(mapper);

    renderer_->AddActor(shape->getActor());

    if(shapesByActor_.size() == 0)
        renderer_->ResetCamera();

    // set up box widget
    shape->setBoxWidget(vtkSmartPointer<vtkBoxWidget>::New());
    shape->getBoxWidget()->SetProp3D(shape->getActor());
    shape->getBoxWidget()->SetInteractor(qvtkWidget->GetRenderWindow()->GetInteractor());
    shape->getBoxWidget()->SetPlaceFactor(1.25);

    shape->getBoxWidget()->PlaceWidget();
    vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;

    shape->getBoxWidget()->AddObserver(vtkCommand::InteractionEvent, callback);

    qvtkWidget->GetRenderWindow()->Render();

    // turn box widget on if in transform mode
    // TODO not if other shape is selected
    if(this->radioButtonTransformActors->isChecked()) {
        shape->getBoxWidget()->On();
    } else {
        shape->getBoxWidget()->Off();
    }
    
    shapesByActor_.insert(pair<vtkActor*, Shape*>(shape->getActor(), shape));
    return shape;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkSetup() {
    this->qvtkWidget->installEventFilter(this);
    
    //Connect qvtk widgets with this object
    this->connections_ = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->connections_->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                               vtkCommand::LeftButtonPressEvent,
                               this,
                               SLOT(vtkClickHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)),
                               nullptr, 1.0);
    this->connections_->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                                vtkCommand::RightButtonPressEvent,
                                this,
                                SLOT(vtkClickHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)),
                                nullptr, 1.0);
    this->connections_->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                                vtkCommand::MouseMoveEvent,
                                this,
                                SLOT(vtkMouseMoveHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)),
                                nullptr, 1.0);
    
    
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    
    this->correspondencePicker_ = new CorrespondencePicker(renderer_);
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer_);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {
    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    // Get the location of the click (in window coordinates)
    int* pos =interactor->GetEventPosition();
   
    //Get global position of the click
    int* size = interactor->GetSize();
    QPoint globalPos = qvtkWidget->mapToGlobal(QPoint(pos[0], size[1]-pos[1]));

    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.005);
    
    // Pick from this location.
    picker->Pick(pos[0], pos[1], 0, renderer_);

    // check if pick was valid
    if(picker->GetCellId() != -1) {
        unordered_map<vtkActor*, Correspondence*>::iterator it = correspondencesByActor_.find(picker->GetActor());
        if(it != correspondencesByActor_.end()) {
            vtkCorrespondenceClicked(it->second, picker->GetCellId(), globalPos, vtkEvent, command);
        } else {
            unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.find(picker->GetActor());
            if(it != shapesByActor_.end()) {
                vtkShapeClicked(it->second, picker->GetCellId(), globalPos, vtkEvent, command);
            }
        }
    }
}

void ShapeAnalyzer::vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {
    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    // Get the location of the click (in window coordinates)
    int* pos =interactor->GetEventPosition();
    correspondencePicker_->mouseMoveHandler(pos[0], pos[1]);
}

void ShapeAnalyzer::vtkCorrespondenceClicked(Correspondence* correspondence, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command) {
    command->AbortFlagOn();
    for(int i = 0; i < listCorrespondences->count(); i++) {
        if(dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i))->getCorrespondence() == correspondence) {
            listCorrespondences->setCurrentRow(i);
            if(vtkEvent == vtkCommand::RightButtonPressEvent) {
                qtShowContextMenuCorrepondences(pos);
            }
        }
    }
}

void ShapeAnalyzer::vtkShapeClicked(Shape *shape, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command) {
    if(this->radioButtonAddCorrespondences->isChecked() && vtkEvent == vtkCommand::LeftButtonPressEvent) {
        //initialized by correspondencePicker
        Correspondence* correspondence;
        
        // if two corresponding nodes have been picked
        if(correspondencePicker_->pick(&correspondence, shape, cellId)) {
            correspondencesByActor_.insert(pair<vtkActor*, Correspondence*>(correspondence->getActor(), correspondence));
            
            // add to qt
            lastInsertCorresondenceID_++;
            std::string name = "Correspondence ";
            name.append(std::to_string(lastInsertCorresondenceID_));
            // add shape to qt list widget
            CorrespondenceListItem *item = new CorrespondenceListItem(QString(name.c_str()), correspondence);
            this->listCorrespondences->addItem(item);
        }
    } else {
        //select item in list if clicked on shape
        for(int i = 0; i < listShapes->count(); i++) {
            if(dynamic_cast<ShapeListItem*>(listShapes->item(i))->getShape() == shape) {
                listShapes->setCurrentRow(i);
                if(vtkEvent == vtkCommand::RightButtonPressEvent && !this->radioButtonTransformActors->isChecked()) {
                    command->AbortFlagOn();
                    qtShowContextMenuShapes(pos);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Functions managing data structures
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::clear() {
    // qt
    listShapes->disconnect();
    listCorrespondences->disconnect();

    //delete all correspondences
    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        CorrespondenceListItem *item = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i));
        Correspondence *correspondence = item->getCorrespondence();
        
        renderer_->RemoveActor(correspondence->getActor());
        correspondencesByActor_.erase(correspondence->getActor());
        delete item;
        delete correspondence;
    }
    
    
    // delete all shapes
    for(int i = listShapes->count()-1; i > -1; i--) {
        //get shape
        ShapeListItem *item = dynamic_cast<ShapeListItem*>(listShapes->item(i));
        Shape *shape = item->getShape();
        
        renderer_->RemoveActor(shape->getActor());
        shape->getBoxWidget()->SetInteractor(nullptr);
        shape->getBoxWidget()->SetProp3D(nullptr);
        
        shapesByActor_.erase(shape->getActor());
        delete item;
        delete shape;
    }
    
    
    lastInsertShapeID_ = 0;
    lastInsertCorresondenceID_ = 0;
    
    correspondencePicker_->clearSelection();
    
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    CorrespondenceListItem *item = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i));

    // vtk
    renderer_->RemoveActor(item->getCorrespondence()->getActor());
    correspondencesByActor_.erase(item->getCorrespondence()->getActor());
    
    delete item->getCorrespondence();
    delete item;
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteShape(int i) {
    listCorrespondences->disconnect();
    listShapes->disconnect();
    
    // qt
    ShapeListItem *item = dynamic_cast<ShapeListItem*>(this->listShapes->item(i));
    Shape* shape = item->getShape();
    
    //if selected remove green triangle
    correspondencePicker_->clearSelection(shape);
    
    //first remove shape from list
    delete item;

    //iterate through the correspondence list BACKWARDS from the end of the list since delete listItem decreases list->count() and to make sure that we get all correspondences that we want to delete. Then remove items from list
    for(int j = listCorrespondences->count()-1; j > -1; j--) {
        //get correspondence
        Correspondence *correspondence = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(j))->getCorrespondence();
        
        //check whether left or right shape of correspondence equals our shape that we want to delete
        if(shape == correspondence->getShape1() || shape == correspondence->getShape2()) {
            //remove actor from qvtk widget
            renderer_->RemoveActor(correspondence->getActor());
            
            //destroy widgetItem object
            delete listCorrespondences->item(j);
            
            //remove correspondence from list
            correspondencesByActor_.erase(correspondence->getActor());
            
            //destroy correspondence object
            delete correspondence;
        }
    }
    
    // delete shape
    renderer_->RemoveActor(shape->getActor());
    shape->getBoxWidget()->SetInteractor(nullptr);
    shape->getBoxWidget()->SetProp3D(nullptr);
    shapesByActor_.erase(shape->getActor());
    delete shape;

    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}


