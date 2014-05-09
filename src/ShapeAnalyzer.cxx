#include "ShapeAnalyzer.h"

// Constructor
ShapeAnalyzer::ShapeAnalyzer() : lastInsertShapeID_(0), lastInsertCorresondenceID_(0) {
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
    connect(this->actionOpenFile,                   SIGNAL(triggered()),
            this,                                   SLOT(slotOpenShape()));
    connect(this->actionAdd_Correspondences,        SIGNAL(triggered()),
            this,                                   SLOT(slotModusCorrespondences()));
    connect(this->actionTransform_Scene,            SIGNAL(triggered()),
            this,                                   SLOT(slotModusScene()));
    connect(this->actionTransform_Actors,           SIGNAL(triggered()),
            this,                                   SLOT(slotModusActors()));
    
    connect(this->actionTransform_Actors,           SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleBoxWidget()));
    connect(this->actionAdd_Correspondences,        SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleBoxWidget()));
    connect(this->actionTransform_Scene,            SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleBoxWidget()));
    
    //delete selected correspondence triangle if mode was changed. This triggers box widget to show up on shape if shape has been selected.
    connect(this->actionTransform_Scene,            SIGNAL(toggled(bool)),
            this,                                   SLOT(slotClearCurrentSelection()));
    connect(this->actionAdd_Correspondences,        SIGNAL(toggled(bool)),
            this,                                   SLOT(slotClearCurrentSelection()));
    connect(this->actionTransform_Actors,           SIGNAL(toggled(bool)),
            this,                                   SLOT(slotClearCurrentSelection()));
    
    connect(this->actionHelp,                       SIGNAL(triggered()),
            this,                                   SLOT(slotOpenHelpWindow()));
    
    //connection of list widgets is done in extra functions since signals of list widgets are disconnected before and reconnected after deletion of list items
    qtConnectListCorrespondences();
    qtConnectListShapes();
    
    // define shortcuts
    QShortcut* shortcutClear = new QShortcut(QKeySequence(tr("Ctrl+X")), this->menubar);
    connect(shortcutClear,                  SIGNAL(activated()),
            this,                           SLOT(slotClear()));
    this->actionClear->setShortcut(shortcutClear->key());
    
    QShortcut* shortcutOpenShape = new QShortcut(QKeySequence(tr("Ctrl+O")), this->menubar);
    connect(shortcutOpenShape,              SIGNAL(activated()),
            this,                           SLOT(slotOpenShape()));
    this->actionOpenFile->setShortcut(shortcutOpenShape->key());
    
    QShortcut* shortcutModusScene = new QShortcut(QKeySequence(tr("Ctrl+T")), this->menubar);
    connect(shortcutModusScene,             SIGNAL(activated()),
            this,                           SLOT(slotModusScene()));
    this->actionTransform_Scene->setShortcut(shortcutModusScene->key());
    
    QShortcut* shortcutModusActors = new QShortcut(QKeySequence(tr("Ctrl+A")), this->menubar);
    connect(shortcutModusActors,            SIGNAL(activated()),
            this,                           SLOT(slotModusActors()));
    this->actionTransform_Actors->setShortcut(shortcutModusActors->key());
    
    QShortcut* shortcutModusCorrespondences = new QShortcut(QKeySequence(tr("Ctrl+C")), this->menubar);
    connect(shortcutModusCorrespondences,   SIGNAL(activated()),
            this,                           SLOT(slotModusCorrespondences()));
    this->actionAdd_Correspondences->setShortcut(shortcutModusCorrespondences->key());
    
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
    
    QMenu myMenu;
    QAction* geodesicAction = myMenu.addAction("Show Geodesics");
    QAction* renameAction   = myMenu.addAction("Rename");
    QAction* deleteAction   = myMenu.addAction("Delete");
    // ...
    
    QAction* selectedItem = myMenu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteShape(this->listShapes->currentRow());
    } else if (selectedItem == geodesicAction) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        vtkGeodesic *geodesic = new vtkGeodesic(item->getShape());
        geodesic->visualizeGeodesic(qvtkWidget);
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
void ShapeAnalyzer::slotModusActors() {
    this->actionTransform_Scene->setChecked(false);
    this->actionTransform_Actors->setChecked(true);
    this->actionAdd_Correspondences->setChecked(false);
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotModusCorrespondences() {
    this->actionTransform_Scene->setChecked(false);
    this->actionTransform_Actors->setChecked(false);
    this->actionAdd_Correspondences->setChecked(true);
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotModusScene() {
    this->actionTransform_Scene->setChecked(true);
    this->actionTransform_Actors->setChecked(false);
    this->actionAdd_Correspondences->setChecked(false);
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
        if(this->actionTransform_Actors->isChecked()) {
            selectedShape->getBoxWidget()->On();
        } else {
            selectedShape->getBoxWidget()->Off();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous) {
    if(this->actionTransform_Actors->isChecked()) {
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
        Correspondence* currentCorrespondence = dynamic_cast<CorrespondenceListItem*>(current)->getCorrespondence();
        currentCorrespondence->getActor()->GetProperty()->SetColor(1, 0, 0);
        currentCorrespondence->getFace1Actor()->GetProperty()->SetColor(1, 0, 0);
        renderer_->AddActor(currentCorrespondence->getFace1Actor());
        currentCorrespondence->getFace2Actor()->GetProperty()->SetColor(1, 0, 0);
        renderer_->AddActor(currentCorrespondence->getFace2Actor());
        
        //if there exists a previous selection set previous correspondence to green
        if(previous != nullptr) {
            Correspondence* previousCorrespondence = dynamic_cast<CorrespondenceListItem*>(previous)->getCorrespondence();
            previousCorrespondence->getActor()->GetProperty()->SetColor(0, 1, 0);
            renderer_->RemoveActor(previousCorrespondence->getFace1Actor());
            renderer_->RemoveActor(previousCorrespondence->getFace2Actor());
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

    //make sure that all faces are triangles
    vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    triangleFilter->SetInputConnection(reader->GetOutputPort());
    triangleFilter->Update();

    //If shape is not connected (This only happens with bad shape data). Find largest connected region and extract it.
    vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    connectivityFilter->SetInputConnection(triangleFilter->GetOutputPort());
    connectivityFilter->SetExtractionModeToLargestRegion();
    connectivityFilter->Update();
    
    //Remove all isolated points.
    vtkSmartPointer<vtkCleanPolyData> cleanPolyData = vtkSmartPointer<vtkCleanPolyData>::New();
    cleanPolyData->SetInputConnection(connectivityFilter->GetOutputPort());
    cleanPolyData->Update();

    //Visualize with normals. Looks smoother ;)
    vtkSmartPointer<vtkPolyDataNormals> polyDataNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    polyDataNormals->SetInputConnection(cleanPolyData->GetOutputPort());
    polyDataNormals->ComputeCellNormalsOn();
    polyDataNormals->Update();
    
    // get vtk actor and add to renderer_
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(polyDataNormals->GetOutputPort());
    
    
    shape->setPolyData(cleanPolyData->GetOutput());
    shape->setPolyDataNormals(polyDataNormals->GetOutput());
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
        Shape* shape = findShapeByActor(picker->GetActor());
        if(shape != nullptr) {
            vtkShapeClicked(shape, picker->GetCellId(), globalPos, vtkEvent, command);
        } else {
            Correspondence* correspondence = findCorrespondenceByActor(picker->GetActor());
            if(correspondence != nullptr) {
                vtkCorrespondenceClicked(correspondence, picker->GetCellId(), globalPos, vtkEvent, command);
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
    if(this->actionAdd_Correspondences->isChecked() && vtkEvent == vtkCommand::LeftButtonPressEvent) {
        
        Correspondence* correspondence; //initialized by correspondencePicker
        
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
                if(vtkEvent == vtkCommand::RightButtonPressEvent && !this->actionTransform_Actors->isChecked()) {
                    command->AbortFlagOn();
                    qtShowContextMenuShapes(pos);
                }
            }
        }
        
        
    }
}

///////////////////////////////////////////////////////////////////////////////
// Functions accessing data structures
///////////////////////////////////////////////////////////////////////////////

Shape* ShapeAnalyzer::findShapeByActor(vtkActor *actor) {
    unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.find(actor);
    if(it != shapesByActor_.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

Correspondence* ShapeAnalyzer::findCorrespondenceByActor(vtkActor *actor) {
    unordered_map<vtkActor*, Correspondence*>::iterator it = correspondencesByActor_.find(actor);
    if(it != correspondencesByActor_.end()) {
        return it->second;
    } else {
        return nullptr;
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
        renderer_->RemoveActor(correspondence->getFace1Actor());
        renderer_->RemoveActor(correspondence->getFace2Actor());
        
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
    renderer_->RemoveActor(item->getCorrespondence()->getFace1Actor());
    renderer_->RemoveActor(item->getCorrespondence()->getFace2Actor());
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
            renderer_->RemoveActor(correspondence->getFace1Actor());
            renderer_->RemoveActor(correspondence->getFace2Actor());
            
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


