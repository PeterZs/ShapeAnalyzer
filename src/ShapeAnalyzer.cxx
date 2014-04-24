#include "ShapeAnalyzer.h"

// Constructor
ShapeAnalyzer::ShapeAnalyzer() {
    this->setupUi(this);

    this->listShapes->setContextMenuPolicy(Qt::CustomContextMenu);
    this->listCorrespondences->setContextMenuPolicy(Qt::CustomContextMenu);

    // Set up action signals and slots
    connect(this->actionExit,                       SIGNAL(triggered()),
            this,                                   SLOT(exit()));
    connect(this->actionResetCamera,                SIGNAL(triggered()),
            this,                                   SLOT(resetCamera()));
    connect(this->actionClear,                      SIGNAL(triggered()),
            this,                                   SLOT(clear()));
    connect(this->clearButton,                      SIGNAL(clicked()),
            this,                                   SLOT(clear()));
    connect(this->openFileButton,                   SIGNAL(clicked()),
            this,                                   SLOT(openShape()));
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(toggleBoxWidget()));
    connect(this->radioButtonAddCorrespondences,    SIGNAL(toggled(bool)),
            this,                                   SLOT(toggleBoxWidget()));
    connect(this->radioButtonTransformScene,        SIGNAL(toggled(bool)),
            this,                                   SLOT(toggleBoxWidget()));
    
    //delete marked correspondence triangle if mode was changed. This triggers box widget to show up on shape if shape has been selected.
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(deleteSelectedTriangle()));
    connect(this->radioButtonAddCorrespondences,       SIGNAL(toggled(bool)),
            this,                                   SLOT(deleteSelectedTriangle()));
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(deleteSelectedTriangle()));
    
    connect(this->actionHelp,                       SIGNAL(triggered()),
            this,                                   SLOT(openHelpWindow()));
    
    //connection of list widgets is done in extra functions since signals of list widgets are disconnected before and reconnected after deletion of list items
    connectListCorrespondences();
    connectListShapes();
    this->setupVTK();
}


void ShapeAnalyzer::connectListCorrespondences() {
    
    int currentRow = listCorrespondences->currentRow();
    
    listCorrespondences->setCurrentRow(-1);
    connect(this->listCorrespondences,              SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(setCurrentCorrespondenceColor(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listCorrespondences,              SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(showContextMenuCorrespondences(const QPoint&)));

    //trigger SIGNAL currentItemChanged
    listCorrespondences->setCurrentRow(currentRow);
}

void ShapeAnalyzer::connectListShapes() {
    int currentRow = listShapes->currentRow();
    listShapes->setCurrentRow(-1);

    connect(this->listShapes,                       SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(setCurrentBoxWidget(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listShapes,                       SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(showContextMenuShapes(const QPoint&)));

    //trigger SIGNAL currentItemChanged
    listShapes->setCurrentRow(currentRow);
}

///////////////////////////////////////////////////////////////////////////////
// Functions handling QT 
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//catch mouse wheel events since they cause strange behavior on macs
bool ShapeAnalyzer::eventFilter(QObject *object, QEvent *event) {
    if (object == qvtkWidget && event->type() == QEvent::Wheel) {
        return true;
    }
    
    return false;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::exit() {
    qApp->exit();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteSelectedTriangle() {
    renderer_->RemoveActor(selectedTriangleActor_);
    waitForTriangleSelection_ = false;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::openHelpWindow() {
    
    QDialog* help = new QDialog(0,0);

    Ui_Dialog helpUi;
    helpUi.setupUi(help);

    help->show();

}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::openShape() {
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.off)"));
    
    if(fileName.count() == 0)
        return;
    
    Shape* shape = addShapeToVTK(fileName);

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
void ShapeAnalyzer::showContextMenuCorrespondences(const QPoint& pos) {
    int currentRow = this->listCorrespondences->currentRow();

    // menu appears only if item was selected
    if(currentRow >= 0) {

        QPoint globalPos = this->listCorrespondences->mapToGlobal(pos);
        // for QAbstractScrollArea and derived classes you would use:
        // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

        QMenu myMenu;
        myMenu.addAction("Delete");
        // ...

        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem) {
            deleteCorrespondence(this->listCorrespondences->currentRow());
        }

    }
}


///////////////////////////////////////////////////////////////////////////////
/// \brief ShapeAnalyzer::showContextMenu
/// opens menu for shapes
void ShapeAnalyzer::showContextMenuShapes(const QPoint& pos) {
    int currentRow = this->listShapes->currentRow();

    // menu appears only if item was selected
    if(currentRow >= 0) {

        QPoint globalPos = this->listShapes->mapToGlobal(pos);
        // for QAbstractScrollArea and derived classes you would use:
        // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

        QMenu myMenu;
        myMenu.addAction("Delete");
        // ...

        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem) {
            deleteShape(currentRow);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::toggleBoxWidget() {
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
void ShapeAnalyzer::setCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous) {
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
// Functions handling VTK
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
Correspondence* ShapeAnalyzer::addCorrespondenceToVTK(Shape* shape1, Shape* shape2, vtkSmartPointer<vtkTriangle> triangle1, vtkSmartPointer<vtkTriangle> triangle2) {
    // create correspondence
    Correspondence *correspondence = new Correspondence(shape1, shape2, triangle1, triangle2);
    correspondences_.push_back(correspondence);
    
    renderer_->AddActor(correspondence->getActor());
    
    return correspondence;
}

///////////////////////////////////////////////////////////////////////////////
Shape* ShapeAnalyzer::addShapeToVTK(QString fileName) {
    Shape* shape = new Shape;

    // read .off file
    vtkSmartPointer<vtkOFFReader> reader = vtkSmartPointer<vtkOFFReader>::New();
    reader->SetFileName(fileName.toUtf8().constData());
    shape->setData(reader->GetOutput());

    //make shure that shape consists of triangles
    vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    triangleFilter->SetInputConnection(reader->GetOutputPort());
    triangleFilter->Update();
    
    // get vtk actor and add to renderer_
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(triangleFilter->GetOutputPort());

    shape->setActor(vtkSmartPointer<vtkActor>::New());
    shape->getActor()->SetMapper(mapper);

    renderer_->AddActor(shape->getActor());

    if(shapes_.size() == 0)
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
    
    shapes_.push_back(shape);
    return shape;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::resetCamera() {
    renderer_->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::setCurrentCorrespondenceColor(QListWidgetItem* current, QListWidgetItem* previous) {
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


Shape* ShapeAnalyzer::findShapeByActor(vtkActor *actor) {
    for(list<Shape*>::iterator it = shapes_.begin(); it != shapes_.end(); it++) {
        if(actor == (*it)->getActor()) {
            return *it;
        }
    }
    
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::setupVTK() {
    this->qvtkWidget->installEventFilter(this);
    
    //Connect qvtk widgets with this object
    this->connections_ = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->connections_->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                               vtkCommand::LeftButtonPressEvent,
                               this,
                               SLOT(vtkClickHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)),
                               nullptr, 1.0);
    
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer_);
    
    selectedTriangleMapper_ = vtkSmartPointer<vtkDataSetMapper>::New();
    selectedTriangleActor_ = vtkSmartPointer<vtkActor>::New();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {
    //cout << "clicked left" <<endl;
    
    vtkRenderWindowInteractor* interactor =
    vtkRenderWindowInteractor::SafeDownCast(caller);
    
    // Get the location of the click (in window coordinates)
    int* pos =interactor->GetEventPosition();
    //command->AbortFlagOn();
    
    vtkSmartPointer<vtkCellPicker> picker =
    vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.0005);
    
    // Pick from this location.
    picker->Pick(pos[0], pos[1], 0, renderer_);
    
     double* worldPosition = picker->GetPickPosition();
     std::cout << "Cell id is: " << picker->GetCellId() << std::endl;
    
    // check if pick was valid
    
    
    if(picker->GetCellId() != -1) {
        std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1] << " " << worldPosition[2] << endl;
        
        // create new Id object for this pick
        vtkSmartPointer<vtkIdTypeArray> ids =
        vtkSmartPointer<vtkIdTypeArray>::New();
        ids->SetNumberOfComponents(1);
        ids->InsertNextValue(picker->GetCellId());
        
        // mark picked node as selected
        vtkSmartPointer<vtkSelectionNode> selectionNode =
        vtkSmartPointer<vtkSelectionNode>::New();
        selectionNode->SetFieldType(vtkSelectionNode::CELL);
        selectionNode->SetContentType(vtkSelectionNode::INDICES);
        selectionNode->SetSelectionList(ids);
        
        vtkSmartPointer<vtkSelection> selection =
        vtkSmartPointer<vtkSelection>::New();
        selection->AddNode(selectionNode);
        
       
        selectedTriangleShape_ = findShapeByActor(picker->GetActor());
        if(selectedTriangleShape_ != nullptr) {
            vtkSmartPointer<vtkExtractSelection> extractSelection =
            vtkSmartPointer<vtkExtractSelection>::New();
            extractSelection->SetInputData(0, selectedTriangleShape_->getData());
            extractSelection->SetInputData(1, selection);
            extractSelection->Update();
            
            // In selection
            vtkSmartPointer<vtkUnstructuredGrid> selected =
            vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());
            
            std::cout << "There are " << selected->GetNumberOfPoints() << " points in the selection." << std::endl;
            std::cout << "There are " << selected->GetNumberOfCells() << " cells in the selection." << std::endl;
            
            if(this->radioButtonAddCorrespondences->isChecked()) {
                //find a better way of throwing exceptions. Maybe use the methods of vtk or QT?
                if(selected->GetNumberOfCells() != 1) {
                    cerr << "Fatal error: selection should contain exactly one cell. Instead it contains '"<< selected->GetNumberOfCells() <<"' cells" << endl;
                    ::exit(-1);
                }
                
                if(selected->GetCellType(0) != VTK_TRIANGLE) {
                    cerr << "Fatal error: selection is not a triangle (type "<< VTK_TRIANGLE <<"'). Instead it is of type "<< selected->GetCellType(0) <<endl;
                    ::exit(-1);
                }
                
                
                // visual response for picked node
                selectedTriangleMapper_->SetInputData(selected);
                
                selectedTriangleActor_->SetMapper(selectedTriangleMapper_);
                selectedTriangleActor_->GetProperty()->EdgeVisibilityOn();
                selectedTriangleActor_->GetProperty()->SetEdgeColor(0, 1, 0);
                selectedTriangleActor_->GetProperty()->SetLineWidth(3);
                
                selectedTriangleActor_->SetUserTransform(selectedTriangleShape_->getActor()->GetUserTransform());
                
                renderer_->AddActor(selectedTriangleActor_);
                
                // depending on whether there was a selection before or not
                if(waitForTriangleSelection_ == false) {
                    // set source_ to current point and wait
                    triangle1_ = vtkTriangle::SafeDownCast(selected->GetCell(0));
                    shape1_ = selectedTriangleShape_;
                    waitForTriangleSelection_ = true;
                } else {
                    // combine source_ and current pick to a correspondence
                    
                    // if picked node is on the same shape as the source,
                    // update and return
                    if(shape1_ == selectedTriangleShape_) {
                        triangle1_ = vtkTriangle::SafeDownCast(selected->GetCell(0));
                        return;
                    }
                    
                    
                    // create correspondence
                    Correspondence *correspondence = addCorrespondenceToVTK(shape1_, selectedTriangleShape_, triangle1_, vtkTriangle::SafeDownCast(selected->GetCell(0)));
                    
                    //remove green triangle and set flag to false again
                    waitForTriangleSelection_ = false;
                    renderer_->RemoveActor(selectedTriangleActor_);
                    
                    // add to qt
                    // add shape to qt list widget
                    lastInsertCorresondenceID_++;
                    std::string name = "Correspondence ";
                    name.append(std::to_string(lastInsertCorresondenceID_));
                    
                    CorrespondenceListItem *item = new CorrespondenceListItem(QString(name.c_str()), correspondence);
                    this->listCorrespondences->addItem(item);
                }
            } else {
                //select item in list if clicked on shape
                for(int i = 0; i < listShapes->count(); i++) {
                    if(dynamic_cast<ShapeListItem*>(listShapes->item(i))->getShape() == selectedTriangleShape_) {
                        listShapes->setCurrentItem(listShapes->item(i));
                    }
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
        correspondences_.remove(correspondence);
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
        
        shapes_.remove(shape);
        delete item;
        delete shape;
    }

    
    lastInsertShapeID_ = 0;
    lastInsertCorresondenceID_ = 0;

    renderer_->RemoveActor(selectedTriangleActor_);
    waitForTriangleSelection_ = false;
    
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    connectListCorrespondences();
    connectListShapes();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    CorrespondenceListItem *item = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i));

    // vtk
    renderer_->RemoveActor(item->getCorrespondence()->getActor());
    correspondences_.remove(item->getCorrespondence());
    
    delete item->getCorrespondence();
    delete item;
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    connectListCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteShape(int i) {
    listCorrespondences->disconnect();
    listShapes->disconnect();
    
    // qt
    ShapeListItem *item = dynamic_cast<ShapeListItem*>(this->listShapes->item(i));
    Shape* shape = item->getShape();
    
    //if selected remove green triangle
    if(waitForTriangleSelection_ == true && selectedTriangleShape_ == shape) {
        renderer_->RemoveActor(selectedTriangleActor_);
        waitForTriangleSelection_ = false;
        
    }
    
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
            correspondences_.remove(correspondence);
            
            //destroy correspondence object
            delete correspondence;
        }
    }
    
    // delete shape
    renderer_->RemoveActor(shape->getActor());
    shape->getBoxWidget()->SetInteractor(nullptr);
    shape->getBoxWidget()->SetProp3D(nullptr);
    shapes_.remove(shape);
    delete shape;

    this->qvtkWidget->GetRenderWindow()->Render();
    
    connectListCorrespondences();
    connectListShapes();
}


