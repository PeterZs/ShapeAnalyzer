#include "ShapeAnalyzer.h"

// Constructor
ShapeAnalyzer::ShapeAnalyzer() : lastInsertShapeID_(0), lastInsertCorresondenceID_(0), pickerCounter_(0) {
    this->setupUi(this);
    
    //Group actions related to different Views. Automatically unselected other members of group.
    //
    this->actionGroupCorrespondenceType_ = new QActionGroup(this);
    
    actionGroupCorrespondenceType_->addAction(this->actionDisplayFaceCorrespondences);
    actionGroupCorrespondenceType_->addAction(this->actionDisplayPointCorrespondences);
    
    
    //Group actions related to different modes.
    this->actionGroupMode_ = new QActionGroup(this);
    actionGroupMode_->addAction(this->actionTransformScene);
    actionGroupMode_->addAction(this->actionTransformShapes);
    actionGroupMode_->addAction(this->actionAddPointCorrespondences);
    actionGroupMode_->addAction(this->actionAddFaceCorrespondences);
    
    
    this->actionGroupShapeDisplayMode_ = new QActionGroup(this);
    actionGroupShapeDisplayMode_->addAction(this->actionShowSurface);
    actionGroupShapeDisplayMode_->addAction(this->actionShowSurfaceNormals);
    actionGroupShapeDisplayMode_->addAction(this->actionShowTriangulatedMesh);
    actionGroupShapeDisplayMode_->addAction(this->actionShowPointCloud);
    
    
    this->actionGroupProjectionMode_ = new QActionGroup(this);
    actionGroupProjectionMode_->addAction(this->actionProjectionPerspective);
    actionGroupProjectionMode_->addAction(this->actionProjectionParallel);
    
    
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
    
    connect(this->actionImportCorrespondences,      SIGNAL(triggered()),
            this,                                   SLOT(slotImportCorrespondences()));
    
    connect(this->actionSaveScene,                  SIGNAL(triggered()),
            this,                                   SLOT(slotSaveScene()));
    
    connect(this->actionExportScene,                SIGNAL(triggered()),
            this,                                   SLOT(slotExportScene()));
    
    connect(this->actionExportCorrespondences,      SIGNAL(triggered()),
            this,                                   SLOT(slotExportCorrespondences()));
    
    connect(this->actionSaveScreenshot,             SIGNAL(triggered()),
            this,                                   SLOT(slotSaveScreenshot()));
    
    // delete correspondence picker visual response if mode was changed.
    connect(this->actionGroupMode_,                 SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotClearCurrentSelection()));
    
    connect(this->actionGroupMode_,                 SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotToggleBoxWidget()));
    
    connect(this->actionGroupMode_,                 SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotModeAddCorrespondences()));

    

    connect(this->actionGroupCorrespondenceType_,   SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotSetCorrespondenceType()));
    
    connect(this->actionGroupShapeDisplayMode_,     SIGNAL(triggered(QAction*)),
                                                    SLOT(slotSetShapeDisplayMode()));
    
    connect(this->actionHelp,                       SIGNAL(triggered()),
            this,                                   SLOT(slotShowHelp()));
    
    connect(this->actionSettings,                   SIGNAL(triggered()),
            this,                                   SLOT(slotShowSettings()));
    
    connect(this->actionSetBackgroundColor,         SIGNAL(triggered()),
            this,                                   SLOT(slotSetBackgroundColor()));
    
    connect(this->actionGroupProjectionMode_,       SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotToggleProjectionMode()));

    
    connect(this->actionAllPointCorrespondences,    SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTabAllPointCorrespondences(bool)));
    
    connect(this->actionAllFaceCorrespondences,     SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTabAllFaceCorrespondences(bool)));
    
    connect(this->hideAllButton,                    SIGNAL(released()),
            this,                                   SLOT(slotHideCorrespondences()));
    
    // tab signals
    connect(this->actionShapeInfo,                  SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTabShapeInfo(bool)));
    connect(this->actionShapeInterpolation,         SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTabShapeInterpolation(bool)));
    
    
    connect(this->actionCorrespondenceColoring,     SIGNAL(toggled(bool)),
            this,                                   SLOT(slotTabCorrespondenceColoring(bool)));
    
    connect(this->tabWidgetCorrespondences,         SIGNAL(currentChanged(int)),
            this,                                   SLOT(slotTabCorrespondencesCurrentChanged(int)));
    
    //register metrics
    Factory<Metric>::getInstance()->Register<EuclideanMetric>("Euclidean Metric");
    Factory<Metric>::getInstance()->Register<GeodesicMetric>("Geodesic Metric");
    
    //register signatures
    Factory<Signature>::getInstance()->Register<HeatKernelSignature>("Heat Kernel Signature");
    Factory<Signature>::getInstance()->Register<WaveKernelSignature>("Wave Kernel Signature");
    Factory<Signature>::getInstance()->Register<GlobalPointSignature>("Global Point Signature");
    
    //register samplings
    Factory<Sampling>::getInstance()->Register<FarthestPointSampling<EuclideanMetric> >("FPS Euclidean Metric");
    Factory<Sampling>::getInstance()->Register<FarthestPointSampling<GeodesicMetric> >("FPS Geodesic Metric");
    
    //register Laplace-Beltrami Operators
    Factory<LaplaceBeltramiOperator>::getInstance()->Register<FEMLaplaceBeltramiOperator>("FEM Laplace-Beltrami Operator");
    
    
    // connection of list widgets is done in extra functions since signals of
    // list widgets are disconnected before and reconnected after deletion of
    // list items
    qtConnectListCorrespondences();
    qtConnectListShapes();
    
    
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
    
    
    slotSetSelectedCurrentCorrespondence(listCorrespondences->currentItem(), nullptr);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtConnectListShapes() {
    connect(this->listShapes,                       SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this,                                   SLOT(slotSetSelectedCurrentShape(QListWidgetItem*, QListWidgetItem*)));
    connect(this->listShapes,                       SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(slotShowContextMenuShapes(const QPoint&)));
    
    //manually call slot set box widget
    slotSetSelectedCurrentShape(listShapes->currentItem(), nullptr);
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::render() {
    this->qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
// Functions handling QT
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
bool ShapeAnalyzer::eventFilter(QObject *object, QEvent *event) {
    //catch and ignore mouse wheel events since they cause strange behavior on Mac OSX
    if (object == qvtkWidget && event->type() == QEvent::Wheel) {
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtAddMetricMenu(QMenu* menu, HashMap<QAction*, string>& entries) {
    QMenu* metricMenu = new QMenu();
    metricMenu->setTitle("Visualize Metric");
    menu->addMenu(metricMenu);
    
    unordered_map<string, string>& labels = Factory<Metric>::getInstance()->getLabels();
    
    for(unordered_map<string, string>::iterator it = labels.begin(); it != labels.end(); it++) {
        entries.add(metricMenu->addAction(it->second.c_str()), it->first);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtAddSignatureMenu(QMenu* menu, HashMap<QAction*, string>& entries) {
    QMenu* signatureMenu = new QMenu();
    signatureMenu->setTitle("Visualize Signature");
    menu->addMenu(signatureMenu);

    
    unordered_map<string, string>& labels = Factory<Signature>::getInstance()->getLabels();
    
    for(unordered_map<string, string>::iterator it = labels.begin(); it != labels.end(); it++) {
        entries.add(signatureMenu->addAction(it->second.c_str()), it->first);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtAddSamplingMenu(QMenu* menu, HashMap<QAction*, string>& entries) {
    QMenu* samplingMenu = new QMenu();
    samplingMenu->setTitle("Samplings");
    menu->addMenu(samplingMenu);

    unordered_map<string, string>& labels = Factory<Sampling>::getInstance()->getLabels();
    
    for(unordered_map<string, string>::iterator it = labels.begin(); it != labels.end(); it++) {
        entries.add(samplingMenu->addAction(it->second.c_str()), it->first);
    }
    
    
    
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
                                              2,
                                              &ok
                                              );
    // change opacity if ok was given
    if (ok) {
        shape->getActor()->GetProperty()->SetOpacity(opacity);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtInputDialogRenameShape(qtListWidgetItem<Shape>* item) {
    bool ok;
    QString name = QInputDialog::getText (
                                           this,
                                           tr("Rename"),
                                           tr("New Name"),
                                           QLineEdit::Normal,
                                           item->text(),
                                           &ok
                                           );
    // rename if ok was given
    if (ok) {
        item->setText(name);
        item->getItem()->setName(name.toStdString());
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtInputDialogRenameCorrespondence(qtListWidgetItem<Correspondence>* item) {
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
        item->getItem()->setLabel(label.toStdString());
        
        // fire event for correspondenceTabs
        for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
            it->second->onCorrespondenceEdit(item->getItem());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowEigenfunction(Shape* shape) {
    bool ok;
    int i = QInputDialog::getInt(
                                                   this,
                                                   tr("Eigenfunction"),
                                                   tr("Choose an eigenfunction."),
                                                   0,
                                                   0,
                                                   std::min((vtkIdType) 99, shape->getPolyData()->GetNumberOfPoints()),
                                                   1,
                                                   &ok
                                                   );
    // show eigenfunction
    if (ok) {
        ScalarPointAttribute eigenfunction(shape);
        
        LaplaceBeltramiOperator* laplacian = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian->initialize(shape, 100);
        laplacian->getEigenfunction(i, eigenfunction);
        delete laplacian;
        
        ScalarPointColoring coloring(shape, eigenfunction);
        coloring.color();
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowHeatDiffusion(Shape* shape) {
    bool ok;
    double t = QInputDialog::getDouble(
                                       this,
                                       tr("Heat diffusion"),
                                       tr("Choose diffusion time."),
                                       0,
                                       0,
                                       2147483647,
                                       2,
                                       &ok
                                       );
    if (!ok) {
        return;
    }
    
    vtkIdType source = QInputDialog::getInt(
                                       this,
                                       tr("Source vertex"),
                                       tr("Choose ID of source vertex."),
                                       0,
                                       0,
                                       shape->getPolyData()->GetNumberOfPoints()-1,
                                       1,
                                       &ok
                                       );

    if (ok) {

        ScalarPointAttribute u0(shape);
        for(vtkIdType i = 0; i < shape->getPolyData()->GetNumberOfPoints(); i++) {
            if(i == source) {
                u0.getScalars()->SetValue(i, 1.0);
            } else {
                u0.getScalars()->SetValue(i, 0.0);
            }
        }
        LaplaceBeltramiOperator* laplacian = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian->initialize(shape, 100);
        
        HeatDiffusion heatDiffusion(shape, laplacian, u0);
        ScalarPointAttribute ut(shape);
        heatDiffusion.getHeat(ut, t);
        delete laplacian;
        ScalarPointColoring coloring(shape, ut);
        coloring.color();
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowSignature(string id, Shape *shape) {
    bool ok;
    int i = QInputDialog::getInt(
                                            this,
                                            tr("Component"),
                                            tr("Choose component."),
                                            0,
                                            0,
                                            99,
                                            1,
                                            &ok
                                            );
    
    if (ok) {
        LaplaceBeltramiOperator* laplacian = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian->initialize(shape, 100);
        
        Signature* s = Factory<Signature>::getInstance()->create(id);
        s->initialize(shape, laplacian, 100);
        
        ScalarPointAttribute component(shape);
        s->getComponent(i, component);
        delete s;
        delete laplacian;
        ScalarPointColoring coloring(shape, component);
        coloring.color();
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowMetricColoring(string id, Shape *shape) {
    bool ok;
    vtkIdType source = QInputDialog::getInt(
                                            this,
                                            tr("Source vertex"),
                                            tr("Choose ID of source vertex."),
                                            0,
                                            0,
                                            shape->getPolyData()->GetNumberOfPoints()-1,
                                            1,
                                            &ok
                                            );
    
    if (ok) {
        Metric* m = Factory<Metric>::getInstance()->create(id);
        m->initialize(shape);
        ScalarPointAttribute distances(shape);
        m->getAllDistances(distances, source);
        ScalarPointColoring coloring(shape, distances);
        coloring.color();
        delete m;
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowSampling(string id, Shape* shape) {
    Sampling* s = Factory<Sampling>::getInstance()->create(id);
    cout << s->getShape()<<endl;
    delete s;
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
        deleteCorrespondence(this->listCorrespondences->currentRow());
    } else if (selectedItem == renameAction) {
        qtInputDialogRenameCorrespondence((qtListWidgetItem<Correspondence>*) this->listCorrespondences->currentItem());
    } else if (selectedItem == hideAction) { // correspondence data is stored, vtkActor is deleted
        hideCorrespondence(this->listCorrespondences->currentRow());
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuShapes(const QPoint &pos) {
    
    QMenu myMenu;
    
    //map containing unique id of metric + corresponding QAction already set with corresponding label
    HashMap<QAction*, string> metrics;
    qtAddMetricMenu(&myMenu, metrics);
    HashMap<QAction*, string> signatures;
    qtAddSignatureMenu(&myMenu, signatures);
    HashMap<QAction*, string> samplings;
    qtAddSamplingMenu(&myMenu, samplings);
    
    QAction* opacityAction  = myMenu.addAction("Set Opacity");
    QAction* renameAction   = myMenu.addAction("Rename");
    QAction* deleteAction   = myMenu.addAction("Delete");
    QAction* laplaceBeltramiAction = myMenu.addAction("Show Laplace-Beltrami eigenfunction");
    QAction* heatDiffusion = myMenu.addAction("Show heat diffusion");
    QAction* transferFunction = myMenu.addAction("Transfer current function");
    QAction* correspondencesIdentity = myMenu.addAction("Create Identity Correspondences");
    
    
    QAction* selectedItem = myMenu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteShape(this->listShapes->currentRow());
    }  else if (selectedItem == renameAction) {
        qtInputDialogRenameShape((qtListWidgetItem<Shape>*) this->listShapes->currentItem());
    } else if (selectedItem == opacityAction) {
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        qtInputDialogOpacity(currentShape);
    } else if (selectedItem == laplaceBeltramiAction) {
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        qtShowEigenfunction(currentShape);
    } else if(selectedItem == heatDiffusion) {
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        
        qtShowHeatDiffusion(currentShape);
    } else if(selectedItem == transferFunction) {
        Shape* shape1;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        shape1 = item->getItem();
        
        vector<ScalarPointAttribute> c1;
        vector<ScalarPointAttribute> c2;
        
        Shape* shape2 = nullptr;
        for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
            if(it->second != shape1) {
                shape2 = it->second;
                break;
            }
        }
        
        Metric* m1;
        m1 = Factory<Metric>::getInstance()->create("geodesic");
        m1->initialize(shape1);
        
        Metric* m2;
        m2 = Factory<Metric>::getInstance()->create("geodesic");
        m2->initialize(shape2);
        
        for(HashMap<PointCorrespondenceData*, bool>::iterator it = pointCorrespondenceData_.begin(); it != pointCorrespondenceData_.end(); it++) {
            PointCorrespondenceData* corr = it->first;
            
            for(int i = 0; i < corr->getShapeIds().size(); i++) {
                if(corr->getShapeIds()[i] == shape1->getId()) {


                    ScalarPointAttribute distances(shape1);
                    m1->getAllDistances(distances, corr->getCorrespondingIds()[i]);
                    c1.push_back(distances);

                }
                
                if(corr->getShapeIds()[i] == shape2->getId()) {

                    ScalarPointAttribute distances(shape2);
                    m2->getAllDistances(distances, corr->getCorrespondingIds()[i]);
                    c2.push_back(distances);
                    
                }
            }
        }
        
        delete m1;
        delete m2;
        
        LaplaceBeltramiOperator* laplacian1 = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian1->initialize(shape1, 100);
        LaplaceBeltramiOperator* laplacian2 = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
        laplacian2->initialize(shape2, 100);
        
        Signature* wks1 = Factory<Signature>::getInstance()->create("wks");
        wks1->initialize(shape1, laplacian1, 200);

        
        Signature* wks2 = Factory<Signature>::getInstance()->create("wks");
        wks2->initialize(shape2, laplacian2, 200);
        
        
        for(int i = 0; i < 125; i++) {
            ScalarPointAttribute wksi1(shape1);
            wks1->getComponent(i, wksi1);
            c1.push_back(wksi1);
            
            ScalarPointAttribute wksi2(shape2);
            wks2->getComponent(i, wksi2);
            c2.push_back(wksi2);
        }
        
        delete wks1;
        delete wks2;
        FunctionalMaps functionalMaps(*shape1, *shape2, laplacian1, laplacian2, c1, c2, 100);
        
        ScalarPointAttribute f(shape1);
        
        for(int i = 0; i < shape1->getPolyData()->GetNumberOfPoints(); i++) {
            double p[3];
            shape1->getPolyData()->vtkDataSet::GetPoint(i, p);
            f.getScalars()->SetValue(i, p[2]);
        }
        ScalarPointColoring coloring1(shape1, f);
        coloring1.color();
        
//        ScalarPointAttribute u0(shape1);
//        vtkIdType source = 1132;
//        for(vtkIdType i = 0; i < shape1->getPolyData()->GetNumberOfPoints(); i++) {
//            if(i == source) {
//                u0.getScalars()->SetValue(i, 1.0);
//            } else {
//                u0.getScalars()->SetValue(i, 0.0);
//            }
//        }
//        HeatDiffusion diffusion(shape1, laplacian1, u0);
//        ScalarPointAttribute f(shape1);
//        diffusion.getHeat(f, 100);
//        ScalarPointColoring coloring1(shape1, f);
//        coloring1.color();
        
        
        ScalarPointAttribute Tf(shape2);
        functionalMaps.transferFunction(f, Tf);
        
        delete laplacian1;
        delete laplacian2;
        
        ScalarPointColoring coloring2(shape2, Tf);
        coloring2.color();
    } else if(selectedItem == correspondencesIdentity) {
        Shape* shape1;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        shape1 = item->getItem();
        QStringList labels;
        for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
            if(it->second->getId() == shape1->getId())
                continue;
            
            QString label = QString::number(it->second->getId());
            label.append(QString::fromStdString(":"+it->second->getName()));
            labels << label;
            
        }
        bool ok;
        QString chosen = QInputDialog::getItem(this,
                                               "Choose a shape",
                                               "Choose a shape:",
                                               labels,
                                               0,
                                               true,
                                               &ok);
        if(!ok) {
            return;
        }
        
        
        Shape* shape2 = nullptr;
        // add chosen shape to vector
        vtkIdType shapeId = chosen.split(':')[0].toInt();
        for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
            if(it->second->getId() == shapeId) {
                shape2 = it->second;
                break;
            }
        }

        
        for(int i = 0; i < min(shape1->getPolyData()->GetNumberOfPoints(), shape2->getPolyData()->GetNumberOfPoints()); i++) {
            PointCorrespondenceData* data = new PointCorrespondenceData(lastInsertCorresondenceID_);
            data->getShapeIds().push_back(shape1->getId());
            data->getShapeIds().push_back(shape2->getId());
            
            data->getCorrespondingIds().push_back(i);
            data->getCorrespondingIds().push_back(i);
            
            lastInsertCorresondenceID_++;
            
            pointCorrespondenceData_.add(data, false);
            
            // fire event for correspondenceTabs
            for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
                it->second->onCorrespondenceAdd(data);
            }
        }
    } else {
        // try if action is identifier for any factory
        Shape* currentShape;
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape> *) this->listShapes->currentItem();
        currentShape = item->getItem();
        
        // Metric
        if(metrics.containsKey(selectedItem)) {
            qtShowMetricColoring(metrics[selectedItem], currentShape);
            
            return;
        }
        
        // Signatures
        if (signatures.containsKey(selectedItem)) {
            qtShowSignature(signatures[selectedItem], currentShape);
            
            return;
        }
        
        // Samplings
        if (samplings.containsKey(selectedItem)) {
            qtShowSampling(samplings[selectedItem], currentShape);
            
            
            return;
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
void ShapeAnalyzer::slotHideCorrespondences() {
    hideCorrespondences();
}

///////////////////////////////////////////////////////////////////////////////
// removes all current face/point correspondences from renderer
// then adds visible point/face correspondences to renderer
void ShapeAnalyzer::slotSetCorrespondenceType() {
    if(this->actionDisplayFaceCorrespondences->isChecked()) {
        this->lineEditCurrentlyDisplayed->setText("Face Correspondences");
        if(!actionAddFaceCorrespondences->isChecked() && actionAddPointCorrespondences->isChecked()) {
            actionAddFaceCorrespondences->setChecked(true);
            actionAddFaceCorrespondences->trigger();
        }
        
        if(tabWidgetCorrespondences->tabText(tabWidgetCorrespondences->currentIndex()) != "All Face Correspondences") {
            tabWidgetCorrespondences->setCurrentIndex(0);
        }
        
        // current picker is deleted
        delete correspondencePicker_;
        correspondencePicker_ = new FaceCorrespondencePicker(renderer_, lastInsertCorresondenceID_);
        

        // vtk: remove all point correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            correspondence->removeFromRenderer();
        }
        // clear list
        this->listCorrespondences->clear();
        
        
        // add face correspondences from before
        for (auto it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
            // add to list
            this->listCorrespondences->addItem(new qtListWidgetItem<Correspondence>(
                                                                                    QString::fromStdString(it->second->getLabel()),
                                                                                    it->second)
                                               );
            
            // add to renderer
            it->second->addToRenderer();
            
        }

        if(faceCorrespondencesByActor_.size() > 0)
            listCorrespondences->setCurrentRow(0);
        
    } else {
        this->lineEditCurrentlyDisplayed->setText("Point Correspondences");
        if(!actionAddPointCorrespondences->isChecked() && actionAddFaceCorrespondences->isChecked()) {
            actionAddPointCorrespondences->setChecked(true);
            actionAddPointCorrespondences->trigger();
        }
        
        if(tabWidgetCorrespondences->tabText(tabWidgetCorrespondences->currentIndex()) != "All Point Correspondences") {
            tabWidgetCorrespondences->setCurrentIndex(0);
        }
        
        // current selection of picker is deleted
        delete correspondencePicker_;
        correspondencePicker_ = new PointCorrespondencePicker(renderer_, lastInsertCorresondenceID_);
        
        // vtk: remove all face correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            correspondence->removeFromRenderer();
        }
        // clear list
        this->listCorrespondences->clear();
        
        
        // add point correspondences from before
        for (auto it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
            // add to list
            this->listCorrespondences->addItem(new qtListWidgetItem<Correspondence>(
                                                                                    QString::fromStdString(it->second->getLabel()),
                                                                                    it->second)
                                               );
            
            // add to renderer
            it->second->addToRenderer();
            
        }
        
        if(pointCorrespondencesByActor_.size() > 0)
            listCorrespondences->setCurrentRow(0);
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotToggleProjectionMode() {
    vtkSmartPointer<vtkCamera> camera = renderer_->GetActiveCamera();
    
    if (!actionProjectionParallel->isChecked())
        camera->ParallelProjectionOff();
    else
        camera->ParallelProjectionOn();
    
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
void ShapeAnalyzer::slotShowHelp() {
    
    QDialog* help = new QDialog(0,0);
    
    Ui_Dialog helpUi;
    helpUi.setupUi(help);
    
    help->show();
    
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
        vtkOpenShape(reader, filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1).toStdString());
    } else if(filename.endsWith(tr(".vert"))) {
        // read .tri .vert files
        vtkSmartPointer<vtkToscaASCIIReader> reader = vtkSmartPointer<vtkToscaASCIIReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        vtkOpenShape(reader, filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1).toStdString());
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
void ShapeAnalyzer::slotImportCorrespondences() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    tr(""),
                                                    tr("Files (*.txt)"));
    
    if(filename.count() == 0)
        return; //TODO Error handling...
    
    SceneWriterReader reader = SceneWriterReader();
    
    if(filename.endsWith(".txt")) {
        vector<Shape*> shapes;
        shapesByActor_.getValues(shapes);
        
        ShapeComparator comp;
        // sort shapes to guarantee some ordering (in this case orderd by ID) which is not provided by unordered_map
        std::sort(shapes.begin(), shapes.end(), comp);
        
        vector<PointCorrespondenceData*> pointCorrespondences;
        vector<FaceCorrespondenceData*> faceCorrespondences;
        reader.importCorrespondences(filename.toStdString(), lastInsertCorresondenceID_, pointCorrespondences, faceCorrespondences, shapes, this);
        
        // insert point correspondences and fire corresponding events if vector not empty
        for(int i = 0; i < pointCorrespondences.size(); i++) {
            pointCorrespondenceData_.add(pointCorrespondences[i], false);
            
            // fire event for correspondenceTabs
            for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
                it->second->onCorrespondenceAdd(pointCorrespondences[i]);
            }
        }
        
        // insert face correspondences and fire corresponding events if vector not empty
        for(int i = 0; i < faceCorrespondences.size(); i++) {
            faceCorrespondenceData_.add(faceCorrespondences[i], false);
            
            // fire event for correspondenceTabs
            for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
                it->second->onCorrespondenceAdd(faceCorrespondences[i]);
            }
        }
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
    SceneWriterReader writer = SceneWriterReader();
    
    QStringList types;
    types << "Point Correspondences" << "Face Correspondences";
    bool ok; // stores if user pressed ok button
    QString save = QInputDialog::getItem(this, tr("Which Correspondences?"), tr("Correspondences"), types, 0, true, &ok);
    
    
    if (ok && save == types.value(0) &&  pointCorrespondenceData_.size() > 0) {
        QString filename = QFileDialog::getSaveFileName(
                                                        this,
                                                        tr("Save file"),
                                                        tr(""),
                                                        tr("ASCII Point Correspondence Files (*.txt)")
                                                        );
        if (!filename.isEmpty()) {
            //sort shapes since result of HashMap.getValues is unordered
            vector<Shape*> shapes;
            shapesByActor_.getValues(shapes);
            
            ShapeComparator comp;
            
            std::sort(shapes.begin(), shapes.end(), comp);
            
            writer.exportPointCorrespondences(pointCorrespondenceData_, shapes, filename.toStdString());
        }
    }
    
    if (ok && save == types.value(1) && faceCorrespondenceData_.size() > 0) {
        QString filename = QFileDialog::getSaveFileName(this,
                                                        tr("Save file"),
                                                        tr(""),
                                                        tr("ASCII Face Correspondence Files (*.txt)"));
        
        if (!filename.isEmpty()) {
            vector<Shape*> shapes;
            shapesByActor_.getValues(shapes);
            
            ShapeComparator comp;
            
            std::sort(shapes.begin(), shapes.end(), comp);
            
            writer.exportFaceCorrespondences(faceCorrespondenceData_, shapes, filename.toStdString());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSaveScreenshot() {
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
void ShapeAnalyzer::slotShowContextMenuCorrespondences(const QPoint& pos) {
    int currentRow = this->listCorrespondences->currentRow();
    
    // menu appears only if item was selected
    if(currentRow >= 0) {
        
        QPoint globalPos = this->listCorrespondences->mapToGlobal(pos);

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

        qtShowContextMenuShapes(globalPos);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTabShapeInfo(bool checked) {
    if (checked) { // add Shape Info Tab
        qtShapeInfoTab* tab;
        if(listShapes->count() > 0) {
            tab = new qtShapeInfoTab(((qtListWidgetItem<Shape>*) this->listShapes->currentItem())->getItem(), this);
        } else {
            tab = new qtShapeInfoTab(this);
        }
       
        int i = this->tabWidgetShapes->addTab(tab, "Shape Info");
        this->tabWidgetShapes->setCurrentIndex(i);
        shapesTabs_.add("qtShapeInfoTab", tab); // add tab to shapesTabs list.
    } else { // remove shape info tab, if it was there
        for(int i = 0; i < this->tabWidgetShapes->count(); i++) {
            if(this->tabWidgetShapes->tabText(i) == "Shape Info") {
                this->tabWidgetShapes->removeTab(i);
                shapesTabs_.remove("qtShapeInfoTab");
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTabShapeInterpolation(bool checked) {
    if (checked) { // add Shape Info Tab
        qtShapeInterpolationTab* tab = new qtShapeInterpolationTab(&shapesByActor_, &pointCorrespondenceData_, this);
        
        int i = this->tabWidgetShapes->addTab(tab, "Shape Interpolation");
        this->tabWidgetShapes->setCurrentIndex(i);
        shapesTabs_.add("qtShapeInterpolationTab", tab); // add tab to shapesTabs list.
    } else { // remove shape info tab, if it was there
        for(int i = 0; i < this->tabWidgetShapes->count(); i++) {
            if(this->tabWidgetShapes->tabText(i) == "Shape Interpolation") {
                this->tabWidgetShapes->removeTab(i);
                shapesTabs_.remove("qtShapeInterpolationTab");
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTabCorrespondenceColoring(bool checked) {
    if (checked) { // add Tab
        qtCorrespondenceColoringTab* tab = new qtCorrespondenceColoringTab(
                                                                           this->listShapes,
                                                                           &shapesByActor_,
                                                                           &faceCorrespondenceData_,
                                                                           &pointCorrespondenceData_,
                                                                           this
                                                                           );
        int i = this->tabWidgetCorrespondences->addTab(tab, "Correspondence Coloring");
        this->tabWidgetCorrespondences->setCurrentIndex(i);
        // correspondencesTabs_.add("qtCorrespondenceColoringTab", tab);
    } else { // remove shape info tab, if it was there
        for(int i = 0; i < this->tabWidgetCorrespondences->count(); i++) {
            if(this->tabWidgetCorrespondences->tabText(i) == "Correspondence Coloring") {
                this->tabWidgetCorrespondences->removeTab(i);
                // correspondencesTabs_.remove("qtCorrespondenceColoringTab");
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTabAllFaceCorrespondences(bool checked) {
    if(checked) {
        qtFaceCorrespondencesTab* tab = new qtFaceCorrespondencesTab(&faceCorrespondenceData_, this);
        int i = this->tabWidgetCorrespondences->addTab((QWidget*) tab, "All Face Correspondences");
        correspondencesTabs_.add("qtFaceCorrespondencesTab", tab);
        this->tabWidgetCorrespondences->setCurrentIndex(i);
    } else {
        for(int i = 0; i < this->tabWidgetCorrespondences->count(); i++) {
            if(this->tabWidgetCorrespondences->tabText(i) == "All Face Correspondences") {
                this->tabWidgetCorrespondences->removeTab(i);
                correspondencesTabs_.remove("qtFaceCorrespondencesTab");
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTabAllPointCorrespondences(bool checked) {
    if(checked) {
        qtPointCorrespondencesTab* tab = new qtPointCorrespondencesTab(&pointCorrespondenceData_, this);
        
        int i = this->tabWidgetCorrespondences->addTab((QWidget*) tab, "All Point Correspondences");
        correspondencesTabs_.add("qtPointCorrespondencesTab", tab);
        this->tabWidgetCorrespondences->setCurrentIndex(i);
    } else {
        for(int i = 0; i < this->tabWidgetCorrespondences->count(); i++) {
            if(this->tabWidgetCorrespondences->tabText(i) == "All Point Correspondences") {
                this->tabWidgetCorrespondences->removeTab(i);
                correspondencesTabs_.remove("qtPointCorrespondencesTab");
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotTabCorrespondencesCurrentChanged(int i) {
    if(correspondencesTabs_.containsKey("qtPointCorrespondencesTab") && this->tabWidgetCorrespondences->tabText(i) == "All Point Correspondences") {
        if(!actionDisplayPointCorrespondences->isChecked()) {
            actionDisplayPointCorrespondences->setChecked(true);
            actionDisplayPointCorrespondences->trigger();
        }

    } else if(correspondencesTabs_.containsKey("qtFaceCorrespondencesTab") && this->tabWidgetCorrespondences->tabText(i) == "All Face Correspondences") {
        if(!actionDisplayFaceCorrespondences->isChecked()) {
            actionDisplayFaceCorrespondences->setChecked(true);
            actionDisplayFaceCorrespondences->trigger();
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
void ShapeAnalyzer::slotModeAddCorrespondences() {
    if(this->actionAddPointCorrespondences->isChecked()) {
        if(!this->actionDisplayPointCorrespondences->isChecked()) {
            this->actionDisplayPointCorrespondences->setChecked(true);
            this->actionDisplayPointCorrespondences->trigger();
        }
        
        if(!actionShowPointCloud->isChecked() && !actionShowSurface->isChecked()) {
            this->actionShowSurface->setChecked(true);
            this->actionShowSurface->trigger();
        }
        
        this->actionShowSurfaceNormals->setEnabled(false);
        this->actionShowPointCloud->setEnabled(actionDisplayPointCorrespondences->isChecked());
        this->actionShowTriangulatedMesh->setEnabled(actionDisplayFaceCorrespondences->isChecked());
    } else if(this->actionAddFaceCorrespondences->isChecked()) {
        if(!this->actionDisplayFaceCorrespondences->isChecked()) {
            this->actionDisplayFaceCorrespondences->setChecked(true);
            this->actionDisplayFaceCorrespondences->trigger();
        }
        
        if(!actionShowTriangulatedMesh->isChecked() && !actionShowSurface->isChecked()) {
            this->actionShowSurface->setChecked(true);
            this->actionShowSurface->trigger();
        }
        
        this->actionShowSurfaceNormals->setEnabled(false);
        this->actionShowPointCloud->setEnabled(actionDisplayPointCorrespondences->isChecked());
        this->actionShowTriangulatedMesh->setEnabled(actionDisplayFaceCorrespondences->isChecked());
    } else {
        this->actionShowSurfaceNormals->setEnabled(true);
        this->actionShowPointCloud->setEnabled(true);
        this->actionShowTriangulatedMesh->setEnabled(true);
    }
}




///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetSelectedCurrentShape(QListWidgetItem* current, QListWidgetItem* previous) {
    if(current != nullptr) {
        Shape* currentShape = ((qtListWidgetItem<Shape>*) current)->getItem();
    
        if(this->actionTransformShapes->isChecked()) {
            currentShape->getBoxWidget()->On();
            
            if(previous != nullptr) {
                ((qtListWidgetItem<Shape>*) previous)->getItem()->getBoxWidget()->Off();
            }
        }
        
        // fire event for shapesTabs
        for(HashMap<string, qtShapesTab*>::iterator it = shapesTabs_.begin(); it != shapesTabs_.end(); it++) {
            it->second->onShapeSelect(currentShape);
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
        Correspondence* currentCorrespondence = ((qtListWidgetItem<Correspondence>*) current)->getItem();
        // fire event for correspondenceTabs
        for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
            it->second->onCorrespondenceSelect(currentCorrespondence);
        }
        
        // set current correspondence red
        currentCorrespondence->setSelected(true);
        
        // update
        this->qvtkWidget->GetRenderWindow()->Render();
    }
    
    //if there exists a previous selection set previous correspondence to green
    if(previous != nullptr) {
        ((qtListWidgetItem<Correspondence>*) previous)->getItem()->setSelected(false);
        
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
    
    //Connect qvtk widget with this object
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
    
    this->connections_->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                                vtkCommand::KeyPressEvent,
                                this,
                                SLOT(vtkKeyPressHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)),
                                nullptr, 1.0);
    
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer_);
    
    
    vtkSmartPointer<ShapeAnalyzerInteractorStyle> style =
    vtkSmartPointer<ShapeAnalyzerInteractorStyle>::New();
    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle( style );
    
    
    
    this->correspondencePicker_ = new PointCorrespondencePicker(renderer_, lastInsertCorresondenceID_);
    
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
void ShapeAnalyzer::vtkOpenShape(vtkPolyDataAlgorithm* reader, string name) {
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
        Shape* shape = new Shape(lastInsertShapeID_, name, polyDataReader->GetOutput(), renderer_);
        shape->initialize();
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
            string name = "Shape ";
            name.append(to_string(lastInsertShapeID_+1));
            Shape* shape = new Shape(lastInsertShapeID_, name, polyDataReader->GetOutput(), renderer_);
            
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
        
        shape->readBinary(is);
        
        vtkAddShape(shape);
        
        // fire event for shapesTabs
        for(HashMap<string, qtShapesTab*>::iterator it = shapesTabs_.begin(); it != shapesTabs_.end(); it++) {
            it->second->onShapeAdd(shape);
        }
        
        qtListWidgetItem<Shape> *item = new qtListWidgetItem<Shape>(QString(shape->getName().c_str()), shape);
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
        shape->readASCII(is);
        vtkAddShape(shape);
        
        // fire event for shapesTabs
        for(HashMap<string, qtShapesTab*>::iterator it = shapesTabs_.begin(); it != shapesTabs_.end(); it++) {
            it->second->onShapeAdd(shape);
        }
        
        // add shape to qt list widget
        qtListWidgetItem<Shape> *item = new qtListWidgetItem<Shape>(QString(shape->getName().c_str()), shape);
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
        ((qtListWidgetItem<Shape>*) listShapes->item(i))->getItem()->writeBinary(os);
    }
    
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkExportScene(string filename) {
    ofstream os(filename);
    
    os << shapesByActor_.size() << endl;
    os << lastInsertShapeID_ << endl;
    for(int i = 0; i < listShapes->count(); i++) {
        ((qtListWidgetItem<Shape>*) listShapes->item(i))->getItem()->writeASCII(os);
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
        
        if(shapesByActor_.containsKey(picker->GetActor())) {
            Shape* shape = shapesByActor_[picker->GetActor()];
            // depending on whether we want to add face or point correspondences provide picker->GetCellId or picker->GetPointId to vtkShapeClicked method
            vtkShapeClicked(shape, (actionAddFaceCorrespondences->isChecked() ? picker->GetCellId() : picker->GetPointId()), globalPos, vtkEvent, command);
        } else {
            if(pointCorrespondencesByActor_.containsKey(picker->GetActor())) {
                vtkCorrespondenceClicked(pointCorrespondencesByActor_[picker->GetActor()], picker->GetPointId(), globalPos, vtkEvent, command);
            }
            if(faceCorrespondencesByActor_.containsKey(picker->GetActor())) {
                vtkCorrespondenceClicked(faceCorrespondencesByActor_[picker->GetActor()], picker->GetPointId(), globalPos, vtkEvent, command);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkKeyPressHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {
    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    
    std::string key = interactor->GetKeySym();
    
    if(key == "a") {
        if(this->actionAddFaceCorrespondences->isChecked() || this->actionAddPointCorrespondences->isChecked()) {
            // add current seleceted correspondence if more than one shape
            // is involved
            if(pickerCounter_ > 1) {
                addCorrespondence();
            }
            
            
        }
    } else if(key == "Escape") {
        correspondencePicker_->clearSelection();
        pickerCounter_ = 0;
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
            break;
        }
    }
}

void ShapeAnalyzer::setSelected(CorrespondenceData* data) {
    if(data == nullptr) {
        return;
    }
    if(data->getType() == "PointCorrespondenceData") {
        if(pointCorrespondenceData_[(PointCorrespondenceData*) data]) {
            for(int i = 0; i < listCorrespondences->count(); i++) {
                if(((qtListWidgetItem<Correspondence>*) listCorrespondences->item(i))->getItem()->getData() == data) {
                    listCorrespondences->setCurrentRow(i);
                    return;
                }
            }
        }
    } else {
        if(faceCorrespondenceData_[(FaceCorrespondenceData*) data]) {
            for(int i = 0; i < listCorrespondences->count(); i++) {
                if(((qtListWidgetItem<Correspondence>*) listCorrespondences->item(i))->getItem()->getData() == data) {
                    listCorrespondences->setCurrentRow(i);
                    return;
                }
            }
        }
    }
    listCorrespondences->setCurrentRow(-1);
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkShapeClicked(Shape* shape, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command) {
    if((this->actionAddFaceCorrespondences->isChecked() || this->actionAddPointCorrespondences->isChecked())&& vtkEvent == vtkCommand::LeftButtonPressEvent) {
        
        int result = correspondencePicker_->addShape(shape, cellId);
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
    // fire event for correspondenceTabs
    for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
        it->second->onClear();
    }

    // fire event for shapesTabs
    for(HashMap<string, qtShapesTab*>::iterator it = shapesTabs_.begin(); it != shapesTabs_.end(); it++) {
        it->second->onClear();
    }
    
    // qt
    listShapes->disconnect();
    listCorrespondences->disconnect();
    
    //delete all items from the list. Remove displayed correspondences from renderer
    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
        Correspondence *correspondence = item->getItem();
        
        delete item;
        correspondence->removeFromRenderer();
    }
    
    //delete all correspondences (not neccessarily all correspondences are in the listWidget)
    for(HashMap<vtkActor*, PointCorrespondence*>::iterator it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
        
        it->second->removeFromRenderer();
        delete it->second;
    }
    pointCorrespondencesByActor_.clear();
    for(HashMap<vtkActor*, FaceCorrespondence*>::iterator it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
        
        it->second->removeFromRenderer();
        delete it->second;
    }
    faceCorrespondencesByActor_.clear();
    
    
    // delete all correspondence data
    for(HashMap<PointCorrespondenceData*, bool>::iterator it = pointCorrespondenceData_.begin(); it != pointCorrespondenceData_.end(); it++) {
        
        delete it->first;
    }
    pointCorrespondenceData_.clear();
    for(HashMap<FaceCorrespondenceData*, bool>::iterator it = faceCorrespondenceData_.begin(); it != faceCorrespondenceData_.end(); it++) {
        
        delete it->first;
    }
    faceCorrespondenceData_.clear();
    
    
    
    // delete all shapes
    for(int i = listShapes->count()-1; i > -1; i--) {
        //get shape
        qtListWidgetItem<Shape> *item = (qtListWidgetItem<Shape>*) listShapes->item(i);
        Shape* shape = item->getItem();
        shape->removeFromRenderer();
        
        shapesByActor_.remove(shape->getActor());
        delete item;
        
        delete shape;
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
void ShapeAnalyzer::clearPointCorrespondences() {
    // fire event for correspondenceTabs
    for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
        it->second->onPointCorrespondencesClear();
    }
    
    listCorrespondences->disconnect();
    
    if(actionDisplayPointCorrespondences->isChecked()) {
        //delete all items from the list if point correspondences are currently in the listWidget. Remove displayed correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            delete item;
            correspondence->removeFromRenderer();
        }
    }
    
    //delete all correspondences (not neccessarily all correspondences are in the listWidget)
    for(HashMap<vtkActor*, PointCorrespondence*>::iterator it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
        
        it->second->removeFromRenderer();
        delete it->second;
    }
    pointCorrespondencesByActor_.clear();

    
    // delete all correspondence data
    for(HashMap<PointCorrespondenceData*, bool>::iterator it = pointCorrespondenceData_.begin(); it != pointCorrespondenceData_.end(); it++) {
        
        delete it->first;
    }
    pointCorrespondenceData_.clear();

    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}

void ShapeAnalyzer::clearFaceCorrespondences() {
    // fire event for correspondenceTabs
    for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
        it->second->onFaceCorrespondencesClear();
    }
    
    listCorrespondences->disconnect();
    
    if(actionDisplayFaceCorrespondences->isChecked()) {
        //delete all items from the list if face correspondences are currently in the listWidget. Remove displayed correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            delete item;
            correspondence->removeFromRenderer();
        }
    }
    
    //delete all correspondences (not neccessarily all correspondences are in the listWidget)
    for(HashMap<vtkActor*, FaceCorrespondence*>::iterator it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
        
        it->second->removeFromRenderer();
        delete it->second;
    }
    faceCorrespondencesByActor_.clear();
    
    
    // delete all correspondence data

    for(HashMap<FaceCorrespondenceData*, bool>::iterator it = faceCorrespondenceData_.begin(); it != faceCorrespondenceData_.end(); it++) {
        
        delete it->first;
    }
    faceCorrespondenceData_.clear();
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
    
    // fire event for correspondenceTabs
    for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
        it->second->onCorrespondenceDelete(item->getItem()->getData());
    }
    
    
    // correspondence deleting
    if(item->getItem()->getData()->getType() == "PointCorrespondenceData") {
        pointCorrespondenceData_.remove((PointCorrespondenceData*) item->getItem()->getData());
        pointCorrespondencesByActor_.remove(item->getItem()->getLinesActor());
    } else {
        faceCorrespondenceData_.remove((FaceCorrespondenceData*) item->getItem()->getData());
        faceCorrespondencesByActor_.remove(item->getItem()->getLinesActor());
    }
    

    //delete CorrespondenceData object
    delete item->getItem()->getData();
    
    item->getItem()->removeFromRenderer();
    delete item->getItem();
    delete item;
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::hideCorrespondences() {
    listCorrespondences->disconnect();

    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
        Correspondence *correspondence = item->getItem();
        
        delete item;
        correspondence->removeFromRenderer();
        delete correspondence;
    }
    
    if(actionDisplayPointCorrespondences->isChecked()) {
        // Set flag "displayed" to false for each CorrespondenceData object
        for(HashMap<PointCorrespondenceData*, bool>::iterator it = pointCorrespondenceData_.begin(); it != pointCorrespondenceData_.end(); it++) {
            it->second = false;
        }
        
        pointCorrespondencesByActor_.clear();
    } else {
        // Set flag "displayed" to false for each CorrespondenceData object
        for(HashMap<FaceCorrespondenceData*, bool>::iterator it = faceCorrespondenceData_.begin(); it != faceCorrespondenceData_.end(); it++) {
            it->second = false;
        }
        
        faceCorrespondencesByActor_.clear();
    }
    this->qvtkWidget->GetRenderWindow()->Render();
    
    

    
    qtConnectListCorrespondences();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::hideCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    qtListWidgetItem<Correspondence> *item = (qtListWidgetItem<Correspondence>*) listCorrespondences->item(i);
    
    
    // correspondence deleting
    if(item->getItem()->getData()->getType() == "PointCorrespondenceData") {
        pointCorrespondenceData_[(PointCorrespondenceData*) item->getItem()->getData()] = false;
        pointCorrespondencesByActor_.remove(item->getItem()->getLinesActor());
    } else {
        faceCorrespondenceData_[(FaceCorrespondenceData*) item->getItem()->getData()] = false;
        faceCorrespondencesByActor_.remove(item->getItem()->getLinesActor());
    }
    
    
    
    item->getItem()->removeFromRenderer();
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
    
    // fire event for shapesTabs
    for(HashMap<string, qtShapesTab*>::iterator it = shapesTabs_.begin(); it != shapesTabs_.end(); it++) {
        it->second->onShapeDelete(shape);
    }
    
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    //first remove shape from list
    delete item;
    
    //first delete all items in the list
    //iterate through the correspondence list backwards beginning from the end of the list since delete listItem decreases list->count() and to make sure that we get all correspondences that we want to delete.
    for(int j = listCorrespondences->count()-1; j > -1; j--) {
        //get correspondence
        Correspondence *correspondence = ((qtListWidgetItem<Correspondence>*) listCorrespondences->item(j))->getItem();
        

        //check whether one of the shapes of correspondence equals our shape that we want to delete
        for(int i = 0; i < correspondence->getShapes().size(); i++) {
            if(shape == correspondence->getShapes()[i]) {
                //destroy widgetItem object
                delete listCorrespondences->item(j);
                break;
            }
        }
    }
    
    //second delete all correspondences belonging to the shape
    for(HashMap<vtkActor*, PointCorrespondence*>::iterator it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end();) {

        
        //check whether one of the shapes of correspondence equals our shape that we want to delete
        bool found = false;
        for(int i = 0; i < it->second->getShapes().size(); i++) {
            if(shape == it->second->getShapes()[i]) {
                found = true;
                it->second->removeFromRenderer();
                delete it->second;
                break;
            }
        }
        
        // call erase instead of remove since we are removing on iterating through the map
        if(found) {
            it = pointCorrespondencesByActor_.getEntries().erase(it);
        } else {
            ++it;
        }
    }
    for(HashMap<vtkActor*, FaceCorrespondence*>::iterator it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end();) {
        
        
        //check whether one of the shapes of correspondence equals our shape that we want to delete
        bool found = false;
        for(int i = 0; i < it->second->getShapes().size(); i++) {
            if(shape == it->second->getShapes()[i]) {
                found = true;
                it->second->removeFromRenderer();
                delete it->second;
                break;
            }
        }
        
        // call erase instead of remove since we are removing on iterating through the map
        if(found) {
            it = faceCorrespondencesByActor_.getEntries().erase(it);
        } else {
            ++it;
        }
    }
    

    //third delete all data that belongs to the shape
    for(HashMap<PointCorrespondenceData*, bool>::iterator it = pointCorrespondenceData_.begin(); it != pointCorrespondenceData_.end();) {
        
        PointCorrespondenceData* data = it->first;
        //check whether one of the shapes of correspondence equals our shape that we want to delete
        
        bool found = false;
        for(int i = 0; i < data->getShapeIds().size(); i++) {
            if(shape->getId() == data->getShapeIds()[i]) {
                // fire event for correspondenceTabs
                for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
                    it->second->onCorrespondenceDelete(data);
                }
                
                found = true;
                delete data;
                break;
            }
        }
        
        // call erase instead of remove since we are removing on iterating through the map
        if(found) {
            it = pointCorrespondenceData_.getEntries().erase(it);
        } else {
            ++it;
        }
    }
    for(HashMap<FaceCorrespondenceData*, bool>::iterator it = faceCorrespondenceData_.begin(); it != faceCorrespondenceData_.end();) {
        
        FaceCorrespondenceData* data = it->first;
        //check whether one of the shapes of correspondence equals our shape that we want to delete
        
        bool found = false;
        for(int i = 0; i < data->getShapeIds().size(); i++) {
            if(shape->getId() == data->getShapeIds()[i]) {
                // fire event for correspondenceTabs
                for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
                    it->second->onCorrespondenceDelete(data);
                }

                found = true;
                delete data;
                break;
            }
        }
        
        if(found) {
            it = faceCorrespondenceData_.getEntries().erase(it);
        } else {
            ++it;
        }
    }

    
    // delete shape
    shape->removeFromRenderer();
    shapesByActor_.remove(shape->getActor());
    
    delete shape;
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtConnectListCorrespondences();
    qtConnectListShapes();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addCorrespondence() {
    Correspondence* correspondence;
    
    // adding to correspondenceByActor, vtkPart
    if(correspondencePicker_->pick(&correspondence)) {
        // add to qt
        lastInsertCorresondenceID_++;

        // adding to face/point data
        if (this->actionAddPointCorrespondences->isChecked()) { // point correspondence
            PointCorrespondence* pointCorrespondence = (PointCorrespondence*) correspondence;
            pointCorrespondenceData_.add(pointCorrespondence->getData(), true);
            
            pointCorrespondencesByActor_.add(pointCorrespondence->getLinesActor(), pointCorrespondence);
            
            // add shape to qt list widget
            qtListWidgetItem<PointCorrespondence> *item = new qtListWidgetItem<PointCorrespondence>(QString(correspondence->getLabel().c_str()), pointCorrespondence);
            this->listCorrespondences->addItem(item);
        } else { // face correspondence
            FaceCorrespondence* faceCorrespondence = (FaceCorrespondence*) correspondence;
            faceCorrespondenceData_.add(faceCorrespondence->getData(), true);
            faceCorrespondencesByActor_.add(faceCorrespondence->getLinesActor(), faceCorrespondence);
            
            
            // add shape to qt list widget
            qtListWidgetItem<FaceCorrespondence> *item = new qtListWidgetItem<FaceCorrespondence>(QString(correspondence->getLabel().c_str()), faceCorrespondence);
            this->listCorrespondences->addItem(item);
        }
        
        // fire event for correspondenceTabs
        for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
            it->second->onCorrespondenceAdd(correspondence->getData());
        }
    }
    
    pickerCounter_ = 0;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::showCorrespondence(CorrespondenceData* data) {
    if(data->getType() == "PointCorrespondenceData") {
        string label = "Correspondence ";
        label+=std::to_string(data->getId()+1);
        PointCorrespondence* correspondence = new PointCorrespondence(renderer_, label, (PointCorrespondenceData*) data, shapesByActor_);
        
        // create actor and add to vtk
        correspondence->initialize();
        correspondence->addToRenderer();
     
        pointCorrespondenceData_[(PointCorrespondenceData*) data] = true;
        
            
        pointCorrespondencesByActor_.add(correspondence->getLinesActor(), correspondence);
        
        // add shape to qt list widget
        qtListWidgetItem<PointCorrespondence> *item = new qtListWidgetItem<PointCorrespondence>(QString(correspondence->getLabel().c_str()), correspondence);
        this->listCorrespondences->addItem(item);
        this->listCorrespondences->setCurrentItem(item);
        
        this->qvtkWidget->GetRenderWindow()->Render();
        
        if(!actionDisplayPointCorrespondences->isChecked()) {
            actionDisplayPointCorrespondences->setChecked(true);
            actionDisplayPointCorrespondences->trigger();
        }
    } else {
        string label = "Correspondence ";
        label+=std::to_string(data->getId()+1);
        FaceCorrespondence* correspondence = new FaceCorrespondence(renderer_, label, (FaceCorrespondenceData*) data, shapesByActor_);
        
        // create actor and add to vtk
        correspondence->initialize();
        correspondence->addToRenderer();
        
        faceCorrespondenceData_[(FaceCorrespondenceData*) data] = true;
        
        
        faceCorrespondencesByActor_.add(correspondence->getLinesActor(), correspondence);
        
        // add shape to qt list widget
        qtListWidgetItem<FaceCorrespondence> *item = new qtListWidgetItem<FaceCorrespondence>(QString(correspondence->getLabel().c_str()), correspondence);
        this->listCorrespondences->addItem(item);
        this->listCorrespondences->setCurrentItem(item);        
        
        
        this->qvtkWidget->GetRenderWindow()->Render();
        
        if(!actionDisplayFaceCorrespondences->isChecked()) {
            actionDisplayFaceCorrespondences->setChecked(true);
            actionDisplayFaceCorrespondences->trigger();
        }
    }
}

void ShapeAnalyzer::samplePointCorrespondences(unsigned int size) {
    if(!actionDisplayPointCorrespondences->isChecked()) {
        actionDisplayPointCorrespondences->setChecked(true);
        actionDisplayPointCorrespondences->trigger();
    }
    hideCorrespondences();
    
    vector<PointCorrespondenceData*> data;
    pointCorrespondenceData_.getRandomSampleKeys(size, data);
    
    for(int i = 0; i < data.size(); i++) {
        string label = "Correspondence ";
        label+=std::to_string(data[i]->getId()+1);
        PointCorrespondence* correspondence = new PointCorrespondence(renderer_, label, data[i], shapesByActor_);
        
        // create actor and add to vtk
        correspondence->initialize();
        correspondence->addToRenderer();
        
        pointCorrespondenceData_[data[i]] = true;
        pointCorrespondencesByActor_.add(correspondence->getLinesActor(), correspondence);
        
        // add shape to qt list widget
        qtListWidgetItem<PointCorrespondence> *item = new qtListWidgetItem<PointCorrespondence>(QString(correspondence->getLabel().c_str()), correspondence);
        this->listCorrespondences->addItem(item);
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
}

void ShapeAnalyzer::sampleFaceCorrespondences(unsigned int size) {
    if(!actionDisplayFaceCorrespondences->isChecked()) {
        actionDisplayFaceCorrespondences->setChecked(true);
        actionDisplayFaceCorrespondences->trigger();
    }
    hideCorrespondences();
    
    vector<FaceCorrespondenceData*> data;
    faceCorrespondenceData_.getRandomSampleKeys(size, data);
    
    for(int i = 0; i < data.size(); i++) {
        string label = "Correspondence ";
        label+=std::to_string(data[i]->getId()+1);
        FaceCorrespondence* correspondence = new FaceCorrespondence(renderer_, label, data[i], shapesByActor_);
        
        // create actor and add to vtk
        correspondence->initialize();
        correspondence->addToRenderer();
        
        faceCorrespondenceData_[data[i]] = true;
        faceCorrespondencesByActor_.add(correspondence->getLinesActor(), correspondence);
        
        // add shape to qt list widget
        qtListWidgetItem<FaceCorrespondence> *item = new qtListWidgetItem<FaceCorrespondence>(QString(correspondence->getLabel().c_str()), correspondence);
        this->listCorrespondences->addItem(item);
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
}

void ShapeAnalyzer::deleteCorrespondence(CorrespondenceData *data) {
    if(data->getType() == "PointCorrespondenceData") {
        if(!actionDisplayPointCorrespondences->isChecked()) {
            actionDisplayPointCorrespondences->setChecked(true);
            actionDisplayPointCorrespondences->trigger();
        }
    } else {
        if(!actionDisplayFaceCorrespondences->isChecked()) {
            actionDisplayFaceCorrespondences->setChecked(true);
            actionDisplayFaceCorrespondences->trigger();
        }
    }
    
    // try to find correspondence in list
    bool found = false;
    for(int i = 0; i < listCorrespondences->count(); i++) {
        if(((qtListWidgetItem<Correspondence>*) listCorrespondences->item(i))->getItem()->getData() == data) {
            //correspondence found: delete list item, Correspondence object and CorrespondenceData object
            listCorrespondences->setCurrentRow(i);
            deleteCorrespondence(i);
            found = true;
            break;
        }
    }
    
    if(!found) { //correspondence is hidden (Correspondence object does not exist): only delete CorrespondenceData
        if(data->getType() == "PointCorrespondenceData") {
            pointCorrespondenceData_.remove((PointCorrespondenceData*) data);
        } else {
            faceCorrespondenceData_.remove((FaceCorrespondenceData*) data);
        }
        
        
        // fire event for correspondenceTabs
        for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
            it->second->onCorrespondenceDelete(data);
        }
        
        // physically delete data object
        delete data;
    }
}

void ShapeAnalyzer::hideCorrespondence(CorrespondenceData *data) {
    if(data->getType() == "PointCorrespondenceData") {
        if(!actionDisplayPointCorrespondences->isChecked()) {
            actionDisplayPointCorrespondences->setChecked(true);
            actionDisplayPointCorrespondences->trigger();
        }
    } else {
        if(!actionDisplayFaceCorrespondences->isChecked()) {
            actionDisplayFaceCorrespondences->setChecked(true);
            actionDisplayFaceCorrespondences->trigger();
        }
    }
    
    // try to find correspondence in list
    for(int i = 0; i < listCorrespondences->count(); i++) {
        if(((qtListWidgetItem<Correspondence>*) listCorrespondences->item(i))->getItem()->getData() == data) {
            //correspondence found: delete list item, Correspondence object but keep data object
            hideCorrespondence(i);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addShape(Shape* shape) {
    vtkAddShape(shape);
    
    // fire event for shapesTabs
    for(HashMap<string, qtShapesTab*>::iterator it = shapesTabs_.begin(); it != shapesTabs_.end(); it++) {
        it->second->onShapeAdd(shape);
    }
    
    // add shape to qt list widget
    lastInsertShapeID_++;
    
    qtListWidgetItem<Shape> *item = new qtListWidgetItem<Shape>(QString(shape->getName().c_str()), shape);
    listShapes->addItem(item);
    
    //make sure that there always is exactly one item selected if there exists at least one item
    listShapes->setCurrentRow(listShapes->count()-1);
    
    
    qvtkWidget->GetRenderWindow()->Render();
}

