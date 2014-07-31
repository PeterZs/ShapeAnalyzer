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
    
    connect(this->actionLoadCorrespondences,        SIGNAL(triggered()),
            this,                                   SLOT(slotLoadCorrespondences()));
    
    connect(this->actionSaveScene,                  SIGNAL(triggered()),
            this,                                   SLOT(slotSaveScene()));

    connect(this->actionExportScene,                SIGNAL(triggered()),
            this,                                   SLOT(slotExportScene()));
    
    connect(this->actionExportCorrespondences,      SIGNAL(triggered()),
            this,                                   SLOT(slotExportCorrespondences()));
    
    connect(this->actionSaveImage,                  SIGNAL(triggered()),
            this,                                   SLOT(slotSaveImage()));
    
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
    
    connect(this->actionSettings,                   SIGNAL(triggered()),
            this,                                   SLOT(slotShowSettings()));
    
    connect(this->actionSetBackgroundColor,         SIGNAL(triggered()),
            this,                                   SLOT(slotSetBackgroundColor()));
    
    connect(this->actionPerspective,                SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTogglePerspectiveMode(bool)));
    
    connect(this->actionParallel,                   SIGNAL(toggled(bool)),
            this,                                   SLOT(slotToggleParallelMode(bool)));
    
    connect(this->actionWindowPointCorrespondences, SIGNAL(triggered()),
            this,                                   SLOT(slotOpenCorrespondenceWindowPoints()));
    
    connect(this->actionWindowFaceCorrespondences,  SIGNAL(triggered()),
            this,                                   SLOT(slotOpenCorrespondenceWindowFaces()));
    
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
    
    // short cut for ending correspondence picking
    QShortcut *shortcut = new QShortcut(Qt::Key_Escape, this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(slotEndCorrespondencePicker()));
    
    
  
    this->vtkSetup();
    
    qtInitializeSettings();
    
    //Initialize Slepc for eigenfunction computation
    SlepcInitializeNoArguments();
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
void ShapeAnalyzer::render() {
    this->qvtkWidget->GetRenderWindow()->Render();
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
vector<QAction*> ShapeAnalyzer::qtAddSamplingMenu(QMenu* menu) {
    QMenu* samplingMenu = new QMenu();
    samplingMenu->setTitle("Sampling");
    menu->addMenu(samplingMenu);
    
    SamplingFactory factory = SamplingFactory();
    
    vector<string> metrics = factory.getIdentifier();
    vector<QAction*> actions;
    
    for(vector<string>::iterator it = metrics.begin(); it != metrics.end(); it++) {
        // add action with Identifier to the menu and store action pointer in
        // return vector
        actions.push_back(samplingMenu->addAction(it->data()));
    }
    
    return actions;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtInitializeSettings() {
    dialogSettings_ = new QDialog(0,0);
    
    //Ui_Settings settingsUi;
    uiSettings_.setupUi(dialogSettings_);
    
    uiSettings_.checkTriangulation->setChecked(true);
    uiSettings_.checkBoxDegenerated->setChecked(true);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtInputDialogOpacity(Shape* shape) {
    bool ok;
    double opacity = QInputDialog::getDouble (
                                             this,
                                             tr("Opacity"),
                                             tr("Choose a value between 0 and 1"),
                                             shape->getActor()->GetProperty()->GetOpacity(),
                                             0.0,
                                             1.0,
                                             0.1,
                                             &ok
                                           );
    // change opacity if ok was given
    if (ok) {
        shape->getActor()->GetProperty()->SetOpacity(opacity);
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
    // rename if ok was given
    if (ok) {
        item->setText(label);
    }
}

///////////////////////////////////////////////////////////////////////////////
vtkIdType ShapeAnalyzer::qtInputDialogChooseEigenfunction(Shape* shape) {
    bool ok;
    vtkIdType eigenfunction = QInputDialog::getInt(
                                              this,
                                              tr("Eigenfunction"),
                                              tr("Choose an eigenfunction"),
                                              0,
                                              0,
                                              std::min((vtkIdType) 99, shape->getPolyData()->GetNumberOfPoints()),
                                              1,
                                              &ok
                                              );
    // change opacity if ok was given
    if (ok) {
        return eigenfunction;
    }
    return -1;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuCorrepondences(const QPoint &pos) {
    QMenu menu;
    QAction* renameAction   = menu.addAction("Rename");
    QAction* hideAction   = menu.addAction("Hide");
    QAction* deleteAction   = menu.addAction("Delete");
    // ...
    
    QAction* selectedItem = menu.exec(pos);
    if (selectedItem == deleteAction) { // deletes correspondence data
        deleteCorrespondence(this->listCorrespondences->currentRow(), true);
    } else if (selectedItem == renameAction) {
        qtInputDialogRename(this->listCorrespondences->currentItem());
    } else if (selectedItem == hideAction) { // correspondence data is stored, vtkActor is deleted
        deleteCorrespondence(this->listCorrespondences->currentRow(), false);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuShapes(const QPoint &pos) {
    
    QMenu myMenu;
    vector<QAction*> metrics = qtAddMetricMenu(&myMenu);
    QAction* opacityAction  = myMenu.addAction("Set Opacity");
    QAction* renameAction   = myMenu.addAction("Rename");
    QAction* deleteAction   = myMenu.addAction("Delete");
    QAction* laplaceBeltramiAction = myMenu.addAction("Show Laplace-Beltrami eigenfunction");
    // ...
    
    QAction* selectedItem = myMenu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteShape(this->listShapes->currentRow());
    }  else if (selectedItem == renameAction) {
        qtInputDialogRename(this->listShapes->currentItem());
    } else if (selectedItem == opacityAction) {
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        qtInputDialogOpacity(currentShape);
    } else if (selectedItem == laplaceBeltramiAction) {
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        int i = qtInputDialogChooseEigenfunction(currentShape);
        if(i == -1) {
            return;
        }
        if(currentShape != currentShape_) {
            currentShape_ = currentShape;
            delete laplacian_;
            laplacian_ = new FEMLaplaceBeltramiOperator(currentShape, 100);
            laplacian_->initialize();
        }
        
        ScalarPointAttribute eigenfunction(currentShape);
        laplacian_->getEigenfunction(i, eigenfunction);
        
        PointColoring coloring(currentShape, &eigenfunction);
        coloring.color();
    } else {
        // try if action is identifier for any factory
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        // Metric
        MetricFactory metricFactory = MetricFactory();
        for(vector<QAction*>::iterator it = metrics.begin(); it != metrics.end(); it++) {
            if (selectedItem == *it) {
                Metric* m = metricFactory.produce(selectedItem->text().toStdString(), currentShape);
                ScalarPointAttribute distances(currentShape);
                m->getAllDistances(distances);
                PointColoring coloring(currentShape, &distances);
                coloring.color();
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
void ShapeAnalyzer::slotEndCorrespondencePicker() {
    if(this->actionAddCorrespondences->isChecked()) {
        
        // add current seleceted correspondence if more than one shape
        // is involved
        if(shapesByActor_.size() > 1) {
            addCorrespondence();
        }
        
        
    }
}


///////////////////////////////////////////////////////////////////////////////
// removes all current face/point correspondences from renderer
// then adds visible point/face correspondences to renderer
void ShapeAnalyzer::slotSetCorrespondenceType() {
    if(this->actionFaceCorrespondences->isChecked()) {
        // current selection of picker is deleted
        delete correspondencePicker_;
        correspondencePicker_ = new FaceCorrespondencePicker(renderer_);
        
        // clear gui
        this->listCorrespondences->disconnect();
        
        // vtk: remove all point correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            correspondence->remove();
        }
        
        // clear list
        this->listCorrespondences->clear();
        
        // add face correspondences from before
        unsigned counter = 0;
        for (auto it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
            // add to list
            string name = "Correspondence ";
            name.append(to_string(counter));
            this->listCorrespondences->addItem(new qtListWidgetItem<Correspondence>(
                                                                                  QString(name.c_str()),
                                                                                it->second)
                                     );
            counter++;
            
            // add to renderer
            it->second->add();
            
        }
        
        qtConnectListCorrespondences();
    } else {
        // current selection of picker is deleted
        delete correspondencePicker_;
        correspondencePicker_ = new PointCorrespondencePicker(renderer_);
        
        // clear gui
        this->listCorrespondences->disconnect();
        
        // vtk: remove all face correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            correspondence->remove();
        }
        
        // clear list
        this->listCorrespondences->clear();
        
        // add point correspondences from before
        unsigned counter = 0;
        for (auto it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
            // add to list
            string name = "Correspondence ";
            name.append(to_string(counter));
            this->listCorrespondences->addItem(new qtListWidgetItem<Correspondence>(
                                                                                    QString(name.c_str()),
                                                                                    it->second)
                                               );
            counter++;
            
            // add to renderer
            it->second->add();
            
        }
        
        qtConnectListCorrespondences();
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTogglePerspectiveMode(bool b) {
    vtkSmartPointer<vtkCamera> camera = renderer_->GetActiveCamera();
    
    this->actionParallel->setChecked(!b);
    
    if (b == true)
        camera->ParallelProjectionOff();
    else
        camera->ParallelProjectionOn();
    
    renderer_->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotToggleParallelMode(bool b) {
    vtkSmartPointer<vtkCamera> camera = renderer_->GetActiveCamera();
    
    this->actionPerspective->setChecked(!b);
    
    if (b == true)
        camera->ParallelProjectionOn();
    else
        camera->ParallelProjectionOff();
    
    renderer_->GetRenderWindow()->Render();
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
void ShapeAnalyzer::slotOpenCorrespondenceWindowFaces() {
    
    qtCorrespondenceWindow* win = new qtCorrespondenceWindow(
                                                             &pointData_,
                                                             &faceData_,
                                                             &pointCorrespondencesByActor_,
                                                             &faceCorrespondencesByActor_,
                                                             this->listCorrespondences,
                                                             this
                                                             );
    win->tabWidget->setCurrentIndex(1);
    win->show();
    
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotOpenCorrespondenceWindowPoints() {
    
    qtCorrespondenceWindow* win = new qtCorrespondenceWindow(
                                                             &pointData_,
                                                             &faceData_,
                                                             &pointCorrespondencesByActor_,
                                                             &faceCorrespondencesByActor_,
                                                             this->listCorrespondences,
                                                             this
                                                             );
    win->show();
    
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotShowSettings() {
    
    dialogSettings_->show();
    
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
void ShapeAnalyzer::slotLoadCorrespondences() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    tr(""),
                                                    tr("Files (*.txt)"));
    
    if(filename.count() == 0)
        return; //TODO Error handling...
    
    SceneReader reader = SceneReader();
    
    if(filename.endsWith(".txt")) {
        reader.loadCorrespondences(filename.toStdString(), &pointData_, &faceData_, this->listShapes);
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
void ShapeAnalyzer::slotExportCorrespondences() {
    SceneReader reader = SceneReader();
    
    QStringList types;
    types << "Point Correspondences" << "Face Correspondences";
    QString save = QInputDialog::getItem(this, tr("Which Correspondences?"), tr("Correspondences"), types);
    
    if (save == types.value(0) &&  pointData_.size() > 0) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("ASCII Point Correspondence Files (*.txt)"));
    
        reader.exportPointCorrespondences(&pointData_, &shapesByActor_, filename.toStdString());
    }
    
    if (save == types.value(1) && faceData_.size() > 0) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("ASCII Face Correspondence Files (*.txt)"));
        
        reader.exportFaceCorrespondences(&faceData_, &shapesByActor_, filename.toStdString());
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSaveImage() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save scene as image"), tr(""), tr("PNG (*.png)"));
    vtkSmartPointer<vtkCamera> oldCamera = renderer_->GetActiveCamera();
    
    // if checked, rerender to fit the whole scene
    if(uiSettings_.checkWholeScene->isChecked()){
        vtkSmartPointer<vtkCamera> newCamera = vtkSmartPointer<vtkCamera>::New();
        newCamera->DeepCopy(oldCamera);
    
        renderer_->SetActiveCamera(newCamera);
        renderer_->ResetCamera();
        renderer_->GetRenderWindow()->Render();
    }
    
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
    vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderer_->GetRenderWindow());
    // transparency settings
    if (!(uiSettings_.checkNoTransparency->isChecked()))
        windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    windowToImageFilter->ShouldRerenderOff();
    windowToImageFilter->Update();
    
    // save in file
    vtkSmartPointer<vtkPNGWriter> writer =
    vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName((char*) filename.toStdString().c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();
    
    // if checked, rerender to fit the whole scene
    if(uiSettings_.checkWholeScene->isChecked()){
        renderer_->SetActiveCamera(oldCamera);
        renderer_->GetRenderWindow()->Render();
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotShapeSelectionChanged(QListWidgetItem* current, QListWidgetItem* previous) {
    // search for Shape Info Tab
    for(int i = 0; i < this->tabWidget->count(); i++) {
        if(this->tabWidget->tabText(i) == "Shape Info") {
            this->tabWidget->removeTab(i);
            this->tabWidget->insertTab(i, new qtShapeInfoTab((qtListWidgetItem<Shape>*) current), "Shape Info");
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
        int i = this->tabWidget->addTab( new qtShapeInfoTab((qtListWidgetItem<Shape>*) this->listShapes->currentItem()), "Shape Info");
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
        Shape* selectedShape = ((qtListWidgetItem<Shape>*) listShapes->currentItem())->getItem();
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
            ((qtListWidgetItem<Shape>*) current)->getItem()->getBoxWidget()->On();
            
            if(previous != nullptr) {
                ((qtListWidgetItem<Shape>*) previous)->getItem()->getBoxWidget()->Off();
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
        ((qtListWidgetItem<Correspondence>*) current)->getItem()->setSelected(true);
        
        //if there exists a previous selection set previous correspondence to green
        if(previous != nullptr) {
            ((qtListWidgetItem<Correspondence>*) previous)->getItem()->setSelected(false);
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
    shapesByActor_.add(shape->getActor(), shape);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkOpenShape(vtkPolyDataAlgorithm* reader) {
    // the following will filter the shape for certain properties
    // filters can be choosen in the settings ui
    vtkAlgorithmOutput* output;
    output = reader->GetOutputPort();
    
    // filter to triangulate shape
    if(uiSettings_.checkTriangulation->isChecked()) {
        //make sure that all faces are triangles
        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(output);
        triangleFilter->Update();
        vtkAlgorithmOutput* output;
        output = triangleFilter->GetOutputPort();
    }
    
    //Remove all isolated points.
    if(uiSettings_.checkBoxDegenerated->isChecked()) {
        vtkSmartPointer<vtkCleanPolyData> cleanPolyData = vtkSmartPointer<vtkCleanPolyData>::New();
        cleanPolyData->SetInputConnection(output);
        cleanPolyData->Update();
        vtkAlgorithmOutput* output;
        output = cleanPolyData->GetOutputPort();
    }
    
    
    //If shape is not connected (This only happens with bad shape data). Find largest connected region and extract it.
    if(!uiSettings_.checkBoxComponents->isChecked()) {
        vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
        vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
        connectivityFilter->SetInputConnection(output);
        connectivityFilter->SetExtractionModeToLargestRegion();
        connectivityFilter->Update();
        vtkAlgorithmOutput* output;
        output = connectivityFilter->GetOutputPort();
        
        // get vtk actor and add to renderer_
        vtkSmartPointer<vtkPolyDataReader> polyDataReader = (vtkPolyDataReader*) output->GetProducer();
        Shape* shape = new Shape(lastInsertShapeID_, polyDataReader->GetOutput(), renderer_);
        
        addShape(shape);
        
    } else { // extract all regions in different shapes
        // yeah theres a lot of overhead here... but it makes problems
        
        vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
        vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
        connectivityFilter->SetInputConnection(output);
        connectivityFilter->SetExtractionModeToAllRegions();
        connectivityFilter->Update();
        int numberOfRegions = connectivityFilter->GetNumberOfExtractedRegions();
        
        // add all regions separatly
        for (int i = 0; i < numberOfRegions; i++) {
            vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
            vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
            connectivityFilter->SetInputConnection(output);
            connectivityFilter->SetExtractionModeToSpecifiedRegions();
            connectivityFilter->AddSpecifiedRegion(i);
            connectivityFilter->Update();
            
            vtkAlgorithmOutput* output2;
            output2 = connectivityFilter->GetOutputPort();
            
            // get vtk actor and add to renderer_
            vtkSmartPointer<vtkPolyDataReader> polyDataReader = (vtkPolyDataReader*) output2->GetProducer();
            Shape* shape = new Shape(lastInsertShapeID_, polyDataReader->GetOutput(), renderer_);
            
            addShape(shape);
            
            connectivityFilter->DeleteSpecifiedRegion(i);
        }
    }
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
        
        qtListWidgetItem<Shape> *item = new qtListWidgetItem<Shape>(QString(name.c_str()), shape);
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
        
        qtListWidgetItem<Shape> *item = new qtListWidgetItem<Shape>(QString(name.c_str()), shape);
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
        ((qtListWidgetItem<Shape>*) listShapes->item(i))->getItem()->write(os);
    }
    
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkExportScene(string filename) {
    ofstream os(filename);
    
    os << shapesByActor_.size() << endl;
    os << lastInsertShapeID_ << endl;
    for(int i = 0; i < listShapes->count(); i++) {
        os << *(((qtListWidgetItem<Shape>*) listShapes->item(i))->getItem());
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
        if(((qtListWidgetItem<Correspondence>*) listCorrespondences->item(i))->getItem() == correspondence) {
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
        
        int result = correspondencePicker_->add(shape, cellId);
        if(result == 1)
            pickerCounter_++;
        else if(result == -1)
            pickerCounter_ = 1;
        else
            ;
        
        // all shapes are in the current correspondence
        if(shapesByActor_.size() > 1 && pickerCounter_ == shapesByActor_.size()) {
            addCorrespondence();
        }
        
        
    } else {
        //select item in list if clicked on shape
        for(int i = 0; i < listShapes->count(); i++) {
            if(((qtListWidgetItem<Shape>*) listShapes->item(i))->getItem() == shape) {
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
    return shapesByActor_.getValue(actor);
}


///////////////////////////////////////////////////////////////////////////////
Correspondence* ShapeAnalyzer::findCorrespondenceByActor(vtkActor *actor) {
    
    if(pointCorrespondencesByActor_.contains(actor)) {
        return pointCorrespondencesByActor_.getValue(actor);
    }
    
    return faceCorrespondencesByActor_.getValue(actor);
}


///////////////////////////////////////////////////////////////////////////////
// Returns pointer to first Correspondence object that contains the given
// CorrespondenceData
// Returns nullptr if no matching correspondence was found
// Linear in number of Correspondences
Correspondence* ShapeAnalyzer::findCorrespondenceByData(CorrespondenceData* data) {
    for(auto it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
        if (it->second->getData() == data) {
            return it->second;
        }
    }
    
    for(auto it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
        if (it->second->getData() == data) {
            return it->second;
        }
    }
    
    return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
// Returns pointer to first Correspondence object that contains the given
// CorrespondenceData
// Returns nullptr if no matching correspondence was found
// Linear in number of FaceCorrespondences
FaceCorrespondence* ShapeAnalyzer::findFaceCorrespondenceByData(FaceCorrespondenceData* data) {
    for(auto it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
        if (it->second->getData() == data) {
            return it->second;
        }
    }
    
    return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
// Returns pointer to first Correspondence object that contains the given
// CorrespondenceData
// Returns nullptr if no matching correspondence was found
// Linear in number of PointCorrespondences
PointCorrespondence* ShapeAnalyzer::findPointCorrespondenceByData(PointCorrespondenceData* data) {
    for(auto it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
        if (it->second->getData() == data) {
            return it->second;
        }
    }
    
    return nullptr;
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
        qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
        Correspondence *correspondence = item->getItem();
        
        delete item;
        correspondence->remove();
        delete correspondence;
    }
    
    pointCorrespondencesByActor_.clear();
    faceCorrespondencesByActor_.clear();
    
    
    // delete all shapes
    for(int i = listShapes->count()-1; i > -1; i--) {
        //get shape
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape>*) listShapes->item(i);
        Shape* shape = item->getItem();
        shape->remove();

        shapesByActor_.remove(shape->getActor());
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
        qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
        Correspondence *correspondence = item->getItem();
        
        delete item;
        correspondence->remove();
        delete correspondence;
    }
    
    pointCorrespondencesByActor_.clear();
    faceCorrespondencesByActor_.clear();
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}

///////////////////////////////////////////////////////////////////////////////
// if deleteData is true, the CorrespondenceData will be deleted too
void ShapeAnalyzer::deleteCorrespondence(int i, bool deleteData = false) {
    // qt
    listCorrespondences->disconnect();
    
    qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);

    
    // correspondence deleting
    if(this->actionPointCorrespondences->isChecked()) {
        pointData_.remove((PointCorrespondenceData*) item->getItem()->getData());
        pointCorrespondencesByActor_.remove(item->getItem()->getLinesActor());
    } else {
        faceData_.remove((FaceCorrespondenceData*) item->getItem()->getData());
        faceCorrespondencesByActor_.remove(item->getItem()->getLinesActor());
    }
    
    if(!deleteData) {
        if(this->actionPointCorrespondences->isChecked()) {
            pointData_.add((PointCorrespondenceData*) item->getItem()->getData(), false);
        } else {
            faceData_.add((FaceCorrespondenceData*) item->getItem()->getData(), false);
        }
    }
    
    item->getItem()->remove();
    delete item->getItem();
    delete item;
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteShape(int i) {
    listCorrespondences->disconnect();
    listShapes->disconnect();
    
    // qt
    qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape>*) this->listShapes->item(i);
    Shape* shape = item->getItem();
    
    
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    //first remove shape from list
    delete item;

    //iterate through the correspondence list BACKWARDS from the end of the list since delete listItem decreases list->count() and to make sure that we get all correspondences that we want to delete. Then remove items from list
    for(int j = listCorrespondences->count()-1; j > -1; j--) {
        //get correspondence
        Correspondence *correspondence = ((qtListWidgetItem<Correspondence>*) listCorrespondences->item(j))->getItem();
        
        //check whether left or right shape of correspondence equals our shape that we want to delete
        for(int i = 0; i < correspondence->getShapes().size(); i++) {
            if(shape == correspondence->getShapes()[i]) {
                //destroy widgetItem object
                delete listCorrespondences->item(j);
                
                //remove correspondence from list
                pointCorrespondencesByActor_.remove(correspondence->getLinesActor());
                faceCorrespondencesByActor_.remove(correspondence->getLinesActor());
                
                //destroy correspondence object
                correspondence->remove();
                delete correspondence;
            }
        }
    }
    
    // delete shape
    shape->remove();
    shapesByActor_.remove(shape->getActor());

    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addCorrespondence() {
    Correspondence* correspondence;
    
    // adding to correspondenceByActor, vtkPart
    if(correspondencePicker_->pick(&correspondence)) {
    
        // adding to face/point data
        if (this->actionPointCorrespondences->isChecked()) { // point correspondence
            PointCorrespondence* point = (PointCorrespondence*) correspondence;
            pointData_.add(point->getData(), true);
            
            pickerCounter_ = 0;
            pointCorrespondencesByActor_.add(point->getLinesActor(), point);
            
            // add to qt
            lastInsertCorresondenceID_++;
            string name = "Correspondence ";
            name.append(std::to_string(lastInsertCorresondenceID_));
            // add shape to qt list widget
            qtListWidgetItem<PointCorrespondence> *item = new qtListWidgetItem<PointCorrespondence>(QString(name.c_str()), point);
            this->listCorrespondences->addItem(item);
        } else { // face correspondence
            FaceCorrespondence* face = (FaceCorrespondence*) correspondence;
            faceData_.add(face->getData(), true);
            
            pickerCounter_ = 0;
            faceCorrespondencesByActor_.add(face->getLinesActor(), face);
            
            // add to qt
            lastInsertCorresondenceID_++;
            string name = "Correspondence ";
            name.append(std::to_string(lastInsertCorresondenceID_));
            // add shape to qt list widget
            qtListWidgetItem<FaceCorrespondence> *item = new qtListWidgetItem<FaceCorrespondence>(QString(name.c_str()), face);
            this->listCorrespondences->addItem(item);
        }
        
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addShape(Shape* shape) {
    vtkAddShape(shape);
    
    // add shape to qt list widget
    lastInsertShapeID_++;
    string name = "Shape ";
    name.append(to_string(lastInsertShapeID_));
    
    qtListWidgetItem<Shape> *item = new qtListWidgetItem<Shape>(QString(name.c_str()), shape);
    listShapes->addItem(item);
    
    //make sure that there always is exactly one item selected if there exists at least one item
    listShapes->setCurrentRow(listShapes->count()-1);
    
    
    qvtkWidget->GetRenderWindow()->Render();
}


