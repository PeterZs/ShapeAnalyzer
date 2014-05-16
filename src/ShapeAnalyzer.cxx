#include "ShapeAnalyzer.h"

// Constructor
ShapeAnalyzer::ShapeAnalyzer() : lastInsertShapeID_(0), lastInsertCorresondenceID_(0) {
    this->setupUi(this);
    
    //Group actions related to different Views. Automatically unselected other members of group.
    //
    this->actionGroupCorrespondenceType = new QActionGroup(this);
    
    actionGroupCorrespondenceType->addAction(this->actionFaceCorrespondences);
    actionGroupCorrespondenceType->addAction(this->actionPointCorrespondences);
    

    //Group actions related to different modes.
    this->actionGroupMode = new QActionGroup(this);
    actionGroupMode->addAction(this->actionTransformScene);
    actionGroupMode->addAction(this->actionTransformShapes);
    actionGroupMode->addAction(this->actionAddCorrespondences);
    
    
    this->actionGroupShapeDisplayMode = new QActionGroup(this);
    actionGroupShapeDisplayMode->addAction(this->actionShowSurfaceNormals);
    actionGroupShapeDisplayMode->addAction(this->actionShowTriangulatedMesh);
    
    
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
    
    //delete correspondence picker visual response if mode was changed. This triggers box widget to show up on shape if shape has been selected.
    connect(this->actionGroupMode,                  SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotClearCurrentSelection()));
    
    connect(this->actionGroupMode,                  SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotToggleBoxWidget()));
    
    connect(this->actionGroupMode,                  SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotAddCorrespondencesMode()));
    
    connect(this->actionGroupCorrespondenceType,    SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotSetCorrespondenceType()));
    
    connect(this->actionGroupShapeDisplayMode,      SIGNAL(triggered(QAction*)),
                                                    SLOT(slotSetShapeDisplayMode()));
    
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
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listCorrespondences,              SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotShowContextMenuCorrespondences(const QPoint&)));

    
    //manually call slot set box widget
    slotSetSelectedCurrentCorrespondence(listCorrespondences->currentItem(), nullptr);
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
    QMenu metricMenu;
    metricMenu.setTitle("Visualize Metric");
    myMenu.addMenu(&metricMenu);
    QAction* euklideanAction = metricMenu.addAction("Euclidean");
    QAction* geodesicAction = metricMenu.addAction("Geodesics");
    QAction* fpsAction      = myMenu.addAction("FPS");
    QAction* voronoiAction  = myMenu.addAction("Voronoi Cells");
    QAction* renameAction   = myMenu.addAction("Rename");
    QAction* deleteAction   = myMenu.addAction("Delete");
    // ...
    
    QAction* selectedItem = myMenu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteShape(this->listShapes->currentRow());
    } else if (selectedItem == geodesicAction) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        
        vtkGeodesic geodesic(item->getShape());
        geodesic.visualizeGeodesic(qvtkWidget);
    } else if (selectedItem == euklideanAction) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        item->getShape()->visualizeEuclidean();
    } else if (selectedItem == renameAction) {
        ;
    } else if (selectedItem == fpsAction) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        item->getShape()->setFPS(10);
    } else if (selectedItem == voronoiAction) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        item->getShape()->visualizeVoronoiCells();
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
void ShapeAnalyzer::slotSetCorrespondenceType() {
    if(this->actionFaceCorrespondences->isChecked()) {
        delete correspondencePicker_;
        correspondencePicker_ = new FaceCorrespondencePicker(renderer_);
        clearCorrespondences();
        labelCorrespondences->setText(tr("Face Correspondences (displayed)"));
    } else {
        delete correspondencePicker_;
        correspondencePicker_ = new PointCorrespondencePicker(renderer_);
        clearCorrespondences();
        labelCorrespondences->setText(tr("Point Correspondences (displayed)"));
    }
}



///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetShapeDisplayMode() {
    if(this->actionShowTriangulatedMesh->isChecked()) {
        for(unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); ++it) {
            it->second->getMapper()->SetInputData(it->second->getPolyData());
            it->second->getActor()->Modified();
        }
    } else {
        for(unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); ++it) {
            it->second->getMapper()->SetInputData(it->second->getPolyDataNormals());
            it->second->getActor()->Modified();
        }
    }
    this->qvtkWidget->GetRenderWindow()->Render();
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
        if(this->actionTransformShapes->isChecked()) {
            selectedShape->getBoxWidget()->On();
        } else {
            selectedShape->getBoxWidget()->Off();
        }
    }
}

void ShapeAnalyzer::slotAddCorrespondencesMode() {
    if(this->actionAddCorrespondences->isChecked()) {
        this->actionShowTriangulatedMesh->setChecked(true);
        this->actionShowTriangulatedMesh->trigger();
        this->actionShowSurfaceNormals->setEnabled(false);
    } else {
        this->actionShowSurfaceNormals->setEnabled(true);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous) {
    if(this->actionTransformShapes->isChecked()) {
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
void ShapeAnalyzer::slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous) {
    // only change color if there is a selection
    if(current != nullptr) {
        // set current correspondence red
        dynamic_cast<CorrespondenceListItem*>(current)->getCorrespondence()->setSelected(true);
        
        //if there exists a previous selection set previous correspondence to green
        if(previous != nullptr) {
            dynamic_cast<CorrespondenceListItem*>(previous)->getCorrespondence()->setSelected(false);
        }
        
        // update
        this->qvtkWidget->GetRenderWindow()->Render();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Functions handling VTK
///////////////////////////////////////////////////////////////////////////////

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
    
    this->correspondencePicker_ = new PointCorrespondencePicker(renderer_);
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer_);
}

///////////////////////////////////////////////////////////////////////////////
Shape* ShapeAnalyzer::vtkAddShape(QString fileName) {
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
    Shape* shape = new Shape(lastInsertShapeID_, cleanPolyData->GetOutput(), polyDataNormals->GetOutput(), renderer_);
    
    if(actionShowSurfaceNormals->isChecked()) {
        shape->getMapper()->SetInputData(polyDataNormals->GetOutput());
    }
    
    if(shapesByActor_.size() == 0)
        renderer_->ResetCamera();


    vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;

    shape->getBoxWidget()->AddObserver(vtkCommand::InteractionEvent, callback);

    qvtkWidget->GetRenderWindow()->Render();
    
    //triggers boxwidget to show up on new shape
    shapesByActor_.insert(pair<vtkActor*, Shape*>(shape->getActor(), shape));
    return shape;
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
    if(picker->GetPointId() != -1) {
        Shape* shape = findShapeByActor(picker->GetActor());
        if(shape != nullptr) {
            vtkShapeClicked(shape, (actionFaceCorrespondences->isChecked() ? picker->GetCellId() : picker->GetPointId()), globalPos, vtkEvent, command);
        } else {
            Correspondence* correspondence = findCorrespondenceByActor(picker->GetActor());
            if(correspondence != nullptr) {
                vtkCorrespondenceClicked(correspondence, picker->GetPointId(), globalPos, vtkEvent, command);
            }
        }
    }
}

void ShapeAnalyzer::vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {
    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    // Get the location of the click (in window coordinates)
    int* pos =interactor->GetEventPosition();
    
    correspondencePicker_->updateLine(pos[0], pos[1]);
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
    if(this->actionAddCorrespondences->isChecked() && vtkEvent == vtkCommand::LeftButtonPressEvent) {
        
        Correspondence* correspondence; //initialized by correspondencePicker
        
        // if two corresponding nodes have been picked
        if(correspondencePicker_->pick(&correspondence, shape, cellId)) {
            correspondencesByActor_.insert(pair<vtkActor*, Correspondence*>(correspondence->getLineActor(), correspondence));
            
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
                if(vtkEvent == vtkCommand::RightButtonPressEvent && !this->actionTransformShapes->isChecked()) {
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
        
        
        correspondencesByActor_.erase(correspondence->getLineActor());
        delete item;
        correspondence->remove();
        delete correspondence;
    }
    
    
    // delete all shapes
    for(int i = listShapes->count()-1; i > -1; i--) {
        //get shape
        ShapeListItem *item = dynamic_cast<ShapeListItem*>(listShapes->item(i));
        Shape *shape = item->getShape();
        shape->remove();

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

void ShapeAnalyzer::clearCorrespondences() {
    listCorrespondences->disconnect();
    
    //delete all correspondences
    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        CorrespondenceListItem *item = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i));
        Correspondence *correspondence = item->getCorrespondence();
        
        
        correspondencesByActor_.erase(correspondence->getLineActor());
        delete item;
        correspondence->remove();
        delete correspondence;
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    CorrespondenceListItem *item = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i));

    // vtk
    correspondencesByActor_.erase(item->getCorrespondence()->getLineActor());
    
    item->getCorrespondence()->remove();
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
            //destroy widgetItem object
            delete listCorrespondences->item(j);
            
            //remove correspondence from list
            correspondencesByActor_.erase(correspondence->getLineActor());
            
            //destroy correspondence object
            correspondence->remove();
            delete correspondence;
        }
    }
    
    // delete shape
    shape->remove();
    shapesByActor_.erase(shape->getActor());
    delete shape;

    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}


