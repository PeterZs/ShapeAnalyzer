#include "ShapeAnalyzer.h"

// Constructor
ShapeAnalyzer::ShapeAnalyzer() : lastInsertShapeID_(0), lastInsertCorresondenceID_(0), pickerCounter_(0) {
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
    actionGroupShapeDisplayMode->addAction(this->actionShowSurface);
    actionGroupShapeDisplayMode->addAction(this->actionShowSurfaceNormals);
    actionGroupShapeDisplayMode->addAction(this->actionShowTriangulatedMesh);
    actionGroupShapeDisplayMode->addAction(this->actionShowPointCloud);
    
    
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
            this,                                   SLOT(slotOpenFile()));
    
    connect(this->actionSaveScene,                  SIGNAL(triggered()),
            this,                                   SLOT(slotSaveScene()));

    connect(this->actionExportScene,                SIGNAL(triggered()),
            this,                                   SLOT(slotExportScene()));
    
    // delete correspondence picker visual response if mode was changed.
    // This triggers box widget to show up on shape if shape has been selected.
    connect(this->actionGroupMode,                  SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotClearCurrentSelection()));
    
    connect(this->actionGroupMode,                  SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotToggleBoxWidget()));
    
    connect(this->actionGroupMode,                  SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotAddCorrespondencesMode()));

    connect(this->actionGroupCorrespondenceType,    SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotAddCorrespondencesMode()));
    
    connect(this->actionGroupCorrespondenceType,    SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotSetCorrespondenceType()));
    
    connect(this->actionGroupShapeDisplayMode,      SIGNAL(triggered(QAction*)),
                                                    SLOT(slotSetShapeDisplayMode()));
    
    connect(this->actionHelp,                       SIGNAL(triggered()),
            this,                                   SLOT(slotOpenHelpWindow()));
    
    connect(this->actionSetBackgroundColor,         SIGNAL(triggered()),
            this,                                   SLOT(slotSetBackgroundColor()));
    
    // tab signals
    connect(this->actionShape_Info,                 SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTabShapeInfo(bool)));
    
    connect(this->listShapes,                       SIGNAL(currentItemChanged ( QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotShapeSelectionChanged(QListWidgetItem*, QListWidgetItem*)));
    
    
    // connection of list widgets is done in extra functions since signals of
    // list widgets are disconnected before and reconnected after deletion of
    // list items
    qtConnectListCorrespondences();
    qtConnectListShapes();
  
    this->vtkSetup();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtConnectListCorrespondences() {
    listCorrespondences->setCurrentRow(-1);
    connect(this->listCorrespondences,              SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentCorrespondence(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listCorrespondences,              SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotShowContextMenuCorrespondences(const QPoint&)));

    
    //manually call slot set box widget
    slotSetSelectedCurrentCorrespondence(listCorrespondences->currentItem(), nullptr);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtConnectListShapes() {
    connect(this->listShapes,                       SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetCurrentBoxWidget(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listShapes,                       SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotShowContextMenuShapes(const QPoint&)));

    slotSetCurrentBoxWidget(listShapes->currentItem(), nullptr);
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
QList<QListWidgetItem *> ShapeAnalyzer::getShapes() {
    return this->listShapes->findItems(QString("*"), Qt::MatchWildcard);
}



///////////////////////////////////////////////////////////////////////////////
// Functions handling QT 
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
bool ShapeAnalyzer::eventFilter(QObject *object, QEvent *event) {
    //catch mouse wheel events since they cause strange behavior on Mac OSX
    if (object == qvtkWidget && event->type() == QEvent::Wheel) {
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
vector<QAction*> ShapeAnalyzer::qtAddMetricMenu(QMenu* menu) {
    QMenu* metricMenu = new QMenu();
    metricMenu->setTitle("Visualize Metric");
    menu->addMenu(metricMenu);
    
    MetricFactory factory = MetricFactory();
    
    vector<string> metrics = factory.getIdentifier();
    vector<QAction*> actions;
    
    for(vector<string>::iterator it = metrics.begin(); it != metrics.end(); it++) {
        // add action with Identifier to the menu and store action pointer in
        // return vector
        actions.push_back(metricMenu->addAction(it->data()));
    }
    
    return actions;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtInputDialogFPS() {
    bool ok;
    int samples = QInputDialog::getInt (
                                        this,
                                        tr("FPS"),
                                        tr("Number of Samples"),
                                        10, // value
                                        1, // min
                                        100, // max
                                        1, // step size
                                        &ok
                                        );
    // calculate fps if ok was given
    if (ok) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        item->getShape()->setFPS(samples);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtInputDialogRename(QListWidgetItem* item) {
    bool ok;
    QString label = QInputDialog::getText (
                                        this,
                                        tr("Rename"),
                                        tr("New Name"),
                                        QLineEdit::Normal,
                                        item->text(),
                                        &ok
                                        );
    // calculate fps if ok was given
    if (ok) {
        item->setText(label);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuCorrepondences(const QPoint &pos) {
    QMenu menu;
    QAction* renameAction   = menu.addAction("Rename");
    QAction* deleteAction   = menu.addAction("Delete");
    // ...
    
    QAction* selectedItem = menu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteCorrespondence(this->listCorrespondences->currentRow());
    } else if (selectedItem == renameAction) {
        qtInputDialogRename(this->listCorrespondences->currentItem());
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuShapes(const QPoint &pos) {
    
    QMenu myMenu;
    vector<QAction*> metrics = qtAddMetricMenu(&myMenu);
    QAction* fpsAction      = myMenu.addAction("FPS");
    QAction* voronoiAction  = myMenu.addAction("Voronoi Cells");
    QAction* renameAction   = myMenu.addAction("Rename");
    QAction* deleteAction   = myMenu.addAction("Delete");
    // ...
    
    QAction* selectedItem = myMenu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteShape(this->listShapes->currentRow());
    }  else if (selectedItem == renameAction) {
        qtInputDialogRename(this->listShapes->currentItem());
    } else if (selectedItem == fpsAction) {
        qtInputDialogFPS();
    } else if (selectedItem == voronoiAction) {
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        item->getShape()->visualizeVoronoiCells();
    } else {
        // try if action is identifier for any factory
        Shape* currentShape;
        ShapeListItem *item = (ShapeListItem *) this->listShapes->currentItem();
        currentShape = item->getShape();
        
        // Metric
        MetricFactory metricFactory = MetricFactory();
        for(vector<QAction*>::iterator it = metrics.begin(); it != metrics.end(); it++) {
            if (selectedItem == *it) {
                Metric* m = metricFactory.produce(selectedItem->text().toStdString(), currentShape);
                MetricColoring color = MetricColoring(m);
                color.colorShapeFaces();
            }
        }
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
void ShapeAnalyzer::slotSetBackgroundColor() {
    double dcurrentColor[3];
    renderer_->GetBackground(dcurrentColor);
    QColor currentColor;
    currentColor.setRed(dcurrentColor[0]*255);
    currentColor.setGreen(dcurrentColor[1]*255);
    currentColor.setBlue(dcurrentColor[2]*255);
    QColor color = QColorDialog::getColor(currentColor);
    renderer_->SetBackground(color.red()/255.0, color.green()/255.0, color.blue()/255.0);
    renderer_->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotClearCurrentSelection() {
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
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
    if(this->actionShowSurface->isChecked()) {
        for(unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); ++it) {
            it->second->getMapper()->SetInputData(it->second->getPolyData());
            it->second->getActor()->GetProperty()->SetRepresentationToSurface();
            it->second->getActor()->GetProperty()->SetColor(1, 1, 1);
            
            it->second->getActor()->Modified();
        }
    } else if(this->actionShowSurfaceNormals->isChecked()) {
        for(unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); ++it) {
            it->second->getMapper()->SetInputData(it->second->getPolyDataNormals()->GetOutput());
            it->second->getActor()->GetProperty()->SetRepresentationToSurface();
            it->second->getActor()->GetProperty()->SetColor(1, 1, 1);
            
            it->second->getActor()->Modified();
        }
    } else if(this->actionShowTriangulatedMesh->isChecked()) {
        for(unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); ++it) {
            it->second->getMapper()->SetInputData(it->second->getPolyData());
            it->second->getActor()->GetProperty()->SetRepresentationToWireframe();
            it->second->getActor()->GetProperty()->SetColor(1, 1, 0);
            
            it->second->getActor()->Modified();
        }
    } else if(this->actionShowPointCloud->isChecked()) {
        for(unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); ++it) {
            it->second->getActor()->GetProperty()->SetPointSize(3);
            it->second->getActor()->GetProperty()->SetRepresentationToPoints();
            it->second->getActor()->GetProperty()->SetColor(0, 0, 1);
            
            it->second->getActor()->Modified();
        }
    } else {
        //
        ;
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
void ShapeAnalyzer::slotOpenFile() {
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    tr(""),
                                                    tr("Files (*.off *.vert *.scene *.txt)"));
    
    if(filename.count() == 0)
        return; //TODO Error handling...
    
    if(filename.endsWith(tr(".off"))) {
        // read .off file
        vtkSmartPointer<vtkOFFReader> reader = vtkSmartPointer<vtkOFFReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        vtkOpenShape(reader);
    } else if(filename.endsWith(tr(".vert"))) {
        // read .tri .vert files
        vtkSmartPointer<vtkToscaASCIIReader> reader = vtkSmartPointer<vtkToscaASCIIReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        vtkOpenShape(reader);
    } else if(filename.endsWith(tr(".scene"))) {
        vtkOpenScene(filename.toStdString());
    } else if(filename.endsWith(".txt")) {
        vtkImportScene(filename.toStdString());
    } else {
        //TODO Error handling
        ;
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSaveScene() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Binary Scene Files (*.scene)"));
    
    vtkSaveScene(filename.toStdString());
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotExportScene() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("ASCII Scene Files (*.txt)"));
    
    vtkExportScene(filename.toStdString());
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotShapeSelectionChanged(QListWidgetItem* current, QListWidgetItem* previous) {
    // search for Shape Info Tab
    for(int i = 0; i < this->tabWidget->count(); i++) {
        if(this->tabWidget->tabText(i) == "Shape Info") {
            this->tabWidget->removeTab(i);
            this->tabWidget->insertTab(i, new qtShapeInfoTab((ShapeListItem*) current), "Shape Info");
            this->tabWidget->setCurrentIndex(i);
        }
    }
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
void ShapeAnalyzer::slotTabShapeInfo(bool checked) {
    if (checked && this->listShapes->currentRow() >= 0) { // add Shape Info Tab
        int i = this->tabWidget->addTab( new qtShapeInfoTab((ShapeListItem*) this->listShapes->currentItem()), "Shape Info");
        this->tabWidget->setCurrentIndex(i);
    } else if(checked && this->listShapes->currentRow() < 0) { // empty Shape Info Tab
        int i = this->tabWidget->addTab( new qtShapeInfoTab(), "Shape Info");
        this->tabWidget->setCurrentIndex(i);
    } else { // remove shape info tab, if it was there
        for(int i = 0; i < this->tabWidget->count(); i++) {
            if(this->tabWidget->tabText(i) == "Shape Info") {
                this->tabWidget->removeTab(i);
            }
        }
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


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotAddCorrespondencesMode() {
    if(this->actionAddCorrespondences->isChecked()) {
        if(this->actionPointCorrespondences->isChecked()) {
            if(!actionShowPointCloud->isChecked() && !actionShowSurface->isChecked()) {
                this->actionShowSurface->setChecked(true);
                this->actionShowSurface->trigger();
            }
        } else if(this->actionFaceCorrespondences->isChecked()) {
            if(!actionShowTriangulatedMesh->isChecked() && !actionShowSurface->isChecked()) {
                this->actionShowSurface->setChecked(true);
                this->actionShowSurface->trigger();
            }
        }
        
        this->actionShowSurfaceNormals->setEnabled(false);
        this->actionShowPointCloud->setEnabled(actionPointCorrespondences->isChecked());
        this->actionShowTriangulatedMesh->setEnabled(actionFaceCorrespondences->isChecked());
    } else {
        this->actionShowSurfaceNormals->setEnabled(true);
        this->actionShowPointCloud->setEnabled(true);
        this->actionShowTriangulatedMesh->setEnabled(true);
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
void ShapeAnalyzer::vtkAddShape(Shape* shape) {
    if(actionShowSurfaceNormals->isChecked()) {
        shape->getMapper()->SetInputData(shape->getPolyDataNormals()->GetOutput());
    }
    
    if(shapesByActor_.size() == 0)
        renderer_->ResetCamera();


    vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;

    shape->getBoxWidget()->AddObserver(vtkCommand::InteractionEvent, callback);
    
    //triggers boxwidget to show up on new shape
    shapesByActor_.insert(pair<vtkActor*, Shape*>(shape->getActor(), shape));
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkOpenShape(vtkPolyDataAlgorithm* reader) {
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
    
    // get vtk actor and add to renderer_
    Shape* shape = new Shape(lastInsertShapeID_, cleanPolyData->GetOutput(), renderer_);
    
    vtkAddShape(shape);
    
    // add shape to qt list widget
    lastInsertShapeID_++;
    string name = "Shape ";
    name.append(to_string(lastInsertShapeID_));
    
    ShapeListItem *item = new ShapeListItem(QString(name.c_str()), shape);
    listShapes->addItem(item);
    
    //make sure that there always is exactly one item selected if there exists at least one item
    listShapes->setCurrentRow(listShapes->count()-1);
    
    
    qvtkWidget->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkOpenScene(string filename) {
    clear();
    
    //open input file stream in binary mode
    ifstream is(filename, ios::binary);
    
    //read number of shapes
    int64_t numberOfShapes;
    is.read(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
    
    
    //read last insert shape ID
    int64_t lastInsertShapeID;
    is.read(reinterpret_cast<char*>(&lastInsertShapeID), sizeof(int64_t));
    lastInsertShapeID_ = lastInsertShapeID;
    
    //read shapes
    for(unsigned int i = 0; i < numberOfShapes; i++) {
        Shape* shape = new Shape(renderer_);

        shape->read(is);
        
        vtkAddShape(shape);
        
        
        // add shape to qt list widget
        string name = "Shape ";
        name.append(to_string(shape->getId()+1));
        
        ShapeListItem *item = new ShapeListItem(QString(name.c_str()), shape);
        listShapes->addItem(item);
        
        //make sure that there always is exactly one item selected if there exists at least one item
        listShapes->setCurrentRow(listShapes->count()-1);
        
    }
    
    is.close();
    
    renderer_->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkImportScene(string filename) {
    clear();
    ifstream is(filename);

    string line;
    
    unsigned int numberOfShapes;
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> numberOfShapes;
    }
    
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> lastInsertShapeID_;
    }
    
    for(unsigned int i = 0; i < numberOfShapes; i++) {
        Shape* shape = new Shape(renderer_);
        is >> *shape;
        vtkAddShape(shape);
        
        
        // add shape to qt list widget
        string name = "Shape ";
        name.append(to_string(shape->getId()+1));
        
        ShapeListItem *item = new ShapeListItem(QString(name.c_str()), shape);
        listShapes->addItem(item);
        
        //make sure that there always is exactly one item selected if there exists at least one item
        listShapes->setCurrentRow(listShapes->count()-1);
        
    }
    
    is.close();
    
    renderer_->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkSaveScene(string filename) {
    ofstream os(filename, ios::binary);
    int64_t numberOfShapes = (int64_t) shapesByActor_.size();
    os.write(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));

    int64_t lastInsertShapeID = (int64_t) lastInsertShapeID_;
    os.write(reinterpret_cast<char*>(&lastInsertShapeID), sizeof(int64_t));
    
    
    for(int i = 0; i < listShapes->count(); i++) {
        dynamic_cast<ShapeListItem*>(listShapes->item(i))->getShape()->write(os);
    }
    
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkExportScene(string filename) {
    ofstream os(filename);
    
    os << shapesByActor_.size() << endl;
    os << lastInsertShapeID_ << endl;
    for(int i = 0; i < listShapes->count(); i++) {
        os << *(dynamic_cast<ShapeListItem*>(listShapes->item(i))->getShape());
    }
    os.close();
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
    if(picker->GetPointId() != -1 && picker->GetCellId() != -1) {
        Shape* shape = findShapeByActor(picker->GetActor());
        if(shape != nullptr) {
            // depending on whether we want to add face or point correspondences provide picker->GetCellId or picker->GetPointId to vtkShapeClicked method
            vtkShapeClicked(shape, (actionFaceCorrespondences->isChecked() ? picker->GetCellId() : picker->GetPointId()), globalPos, vtkEvent, command);
        } else {
            Correspondence* correspondence = findCorrespondenceByActor(picker->GetActor());
            if(correspondence != nullptr) {
                vtkCorrespondenceClicked(correspondence, picker->GetPointId(), globalPos, vtkEvent, command);
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {
    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    // Get the location of the click (in window coordinates)
    int* pos =interactor->GetEventPosition();
    
    correspondencePicker_->updateMouseLine(pos[0], pos[1]);
}


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkShapeClicked(Shape* shape, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command) {
    if(this->actionAddCorrespondences->isChecked() && vtkEvent == vtkCommand::LeftButtonPressEvent) {
        
        Correspondence* correspondence; //initialized by correspondencePicker
        int result = correspondencePicker_->add(shape, cellId);
        if(result == 1)
            pickerCounter_++;
        else if(result == -1)
            pickerCounter_ = 1;
        else
            ;
        
        if(shapesByActor_.size() > 1 && pickerCounter_ == shapesByActor_.size()) {
            if(correspondencePicker_->pick(&correspondence)) {
                pickerCounter_ = 0;
                correspondencesByActor_.insert(pair<vtkActor*, Correspondence*>(correspondence->getLinesActor(), correspondence));
                
                // add to qt
                lastInsertCorresondenceID_++;
                string name = "Correspondence ";
                name.append(std::to_string(lastInsertCorresondenceID_));
                // add shape to qt list widget
                CorrespondenceListItem *item = new CorrespondenceListItem(QString(name.c_str()), correspondence);
                this->listCorrespondences->addItem(item);
            }
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


///////////////////////////////////////////////////////////////////////////////
Shape* ShapeAnalyzer::findShapeByActor(vtkActor *actor) {
    unordered_map<vtkActor*, Shape*>::iterator it = shapesByActor_.find(actor);
    if(it != shapesByActor_.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}


///////////////////////////////////////////////////////////////////////////////
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
        
        
        correspondencesByActor_.erase(correspondence->getLinesActor());
        delete item;
        correspondence->remove();
        delete correspondence;
    }
    
    
    // delete all shapes
    for(int i = listShapes->count()-1; i > -1; i--) {
        //get shape
        ShapeListItem *item = dynamic_cast<ShapeListItem*>(listShapes->item(i));
        Shape* shape = item->getShape();
        shape->remove();

        shapesByActor_.erase(shape->getActor());
        delete item;
    }
    
    
    lastInsertShapeID_ = 0;
    lastInsertCorresondenceID_ = 0;
    
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::clearCorrespondences() {
    listCorrespondences->disconnect();
    
    //delete all correspondences
    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        CorrespondenceListItem *item = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(i));
        Correspondence *correspondence = item->getCorrespondence();
        
        
        correspondencesByActor_.erase(correspondence->getLinesActor());
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
    correspondencesByActor_.erase(item->getCorrespondence()->getLinesActor());
    
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
    
    
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    //first remove shape from list
    delete item;

    //iterate through the correspondence list BACKWARDS from the end of the list since delete listItem decreases list->count() and to make sure that we get all correspondences that we want to delete. Then remove items from list
    for(int j = listCorrespondences->count()-1; j > -1; j--) {
        //get correspondence
        Correspondence *correspondence = dynamic_cast<CorrespondenceListItem*>(listCorrespondences->item(j))->getCorrespondence();
        
        //check whether left or right shape of correspondence equals our shape that we want to delete
        for(int i = 0; i < correspondence->getShapes().size(); i++) {
            if(shape == correspondence->getShapes()[i]) {
                //destroy widgetItem object
                delete listCorrespondences->item(j);
                
                //remove correspondence from list
                correspondencesByActor_.erase(correspondence->getLinesActor());
                
                //destroy correspondence object
                correspondence->remove();
                delete correspondence;
            }
        }
    }
    
    // delete shape
    shape->remove();
    shapesByActor_.erase(shape->getActor());

    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}


