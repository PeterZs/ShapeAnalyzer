#include "ShapeAnalyzer.h"

// Constructor
ShapeAnalyzer::ShapeAnalyzer() : faceCorrespondencesByActor_(1000), pointCorrespondencesByActor_(1000), faceCorrespondenceData_(100000), pointCorrespondenceData_(100000), lastInsertShapeID_(0), lastInsertCorresondenceID_(0), pickerCounter_(0) {
    this->setupUi(this);
    
    //Group actions related to different Views. Automatically unselected other members of group.
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
    
    connect(this->actionOpenScene,                  SIGNAL(triggered()),
            this,                                   SLOT(slotOpenScene()));

    connect(this->actionImportShape,                SIGNAL(triggered()),
            this,                                   SLOT(slotImportShape()));
    
    connect(this->actionImportCorrespondences,      SIGNAL(triggered()),
            this,                                   SLOT(slotImportCorrespondences()));
    
    connect(this->actionSaveScene,                  SIGNAL(triggered()),
            this,                                   SLOT(slotSaveScene()));
    
    connect(this->actionExportCorrespondences,      SIGNAL(triggered()),
            this,                                   SLOT(slotExportCorrespondences()));
    
    connect(this->actionSaveScreenshot,             SIGNAL(triggered()),
            this,                                   SLOT(slotSaveScreenshot()));
    
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
    
    connect(this->actionSetBackgroundColor,         SIGNAL(triggered()),
            this,                                   SLOT(slotSetBackgroundColor()));
    
    connect(this->actionGroupProjectionMode_,       SIGNAL(triggered(QAction*)),
            this,                                   SLOT(slotToggleProjectionMode()));
    
    connect(this->actionShowScalarBar,              SIGNAL(toggled(bool)),
            this,                                   SLOT(slotShowScalarBar(bool)));
    
    connect(this->buttonHideCorrespondences,        SIGNAL(released()),
            this,                                   SLOT(slotHideVisibleCorrespondences()));
    
    connect(this->buttonSampleCorrespondences,      SIGNAL(released()),
            this,                                   SLOT(slotSampleCorrespondences()));
    
    connect(this->buttonClearAllCorrespondences,    SIGNAL(released()),
            this,                                   SLOT(slotClearAllCorrespondences()));

    
    // connection of list widgets is done in extra functions since signals of
    // list widgets are disconnected before and reconnected after deletion of
    // list items
    qtConnectListCorrespondences();
    qtConnectListShapes();

    this->vtkSetup();
    
    RegisterCustomClasses::Register();
    
    //Initialize Slepc for eigenfunction computation
    SlepcInitializeNoArguments();
    
    qtCreateMenuCustomTabs();
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
void ShapeAnalyzer::qtInputDialogRenameShape(CustomListWidgetItem<Shape>* item) {
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
        scalarBar_->SetTitle(name.toStdString().c_str());
        
        item->setText(name);
        item->getItem()->setName(name.toStdString());
        
        
        // fire event for shapeTabs
        for(auto entry : customTabs_) {
            entry.second->onShapeEdit(item->getItem());
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtExportShapeDialog(Shape* shape) {
    QStringList types;
    types << "OFF" << "Tosca"<< "PLY (ASCII)" <<"PLY (Binary)"<<"OBJ";
    
    bool ok;
    QString type = QInputDialog::getItem(this, "Which file type?", "Write to file", types, 0, true, &ok);
    
    if(!ok) {
        return;
    }
    
    if(type == "OFF") {
        vtkSmartPointer<vtkOFFWriter> writer = vtkSmartPointer<vtkOFFWriter>::New();
        writer->SetInputData(shape->getPolyData());
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("OFF Files (*.off)"));
        if(filename.isEmpty()) {
            return;
        }
        writer->SetFileName(filename.toStdString().c_str());
        writer->Write();
    } else if(type == "Tosca") {
        vtkSmartPointer<vtkToscaWriter> writer = vtkSmartPointer<vtkToscaWriter>::New();
        writer->SetInputData(shape->getPolyData());
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Tosca Files (*.vert)"));
        if(filename.isEmpty()) {
            return;
        }
        writer->SetFileName(filename.toStdString().c_str());
        writer->Write();
    } else if(type == "PLY (ASCII)") {
        vtkSmartPointer<vtkPLYWriter> writer = vtkSmartPointer<vtkPLYWriter>::New();
        writer->SetFileTypeToASCII();
        writer->SetInputData(shape->getPolyData());
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("ASCII PLY Files (*.ply)"));
        if(filename.isEmpty()) {
            return;
        }
        writer->SetFileName(filename.toStdString().c_str());
        writer->Write();
    } else if(type == "PLY (Binary)") {
        vtkSmartPointer<vtkPLYWriter> writer = vtkSmartPointer<vtkPLYWriter>::New();
        writer->SetFileTypeToBinary();
        writer->SetInputData(shape->getPolyData());
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Binary PLY Files (*.ply)"));
        if(filename.isEmpty()) {
            return;
        }
        writer->SetFileName(filename.toStdString().c_str());
        writer->Write();
    } else {
        vtkSmartPointer<vtkOBJWriter> writer = vtkSmartPointer<vtkOBJWriter>::New();
        writer->SetInputData(shape->getPolyData());
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("OBJ Files (*.obj)"));
        if(filename.isEmpty()) {
            return;
        }
        writer->SetFileName(filename.toStdString().c_str());
        writer->Write();
    }
}

///////////////////////////////////////////////////////////////////////////////
//void ShapeAnalyzer::qtCreateIdentityCorrespondences(Shape* shape1) {
//    QStringList labels;
//    for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
//        if(it->second->getId() == shape1->getId())
//            continue;
//        
//        QString label = QString::number(it->second->getId());
//        label.append(QString::fromStdString(":"+it->second->getName()));
//        labels << label;
//        
//    }
//    bool ok;
//    QString chosen = QInputDialog::getItem(this,
//                                           "Choose a shape",
//                                           "Choose a shape:",
//                                           labels,
//                                           0,
//                                           true,
//                                           &ok);
//    if(!ok) {
//        return;
//    }
//    
//    
//    Shape* shape2 = nullptr;
//    vtkIdType shapeId = chosen.split(':')[0].toInt();
//    for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
//        if(it->second->getId() == shapeId) {
//            shape2 = it->second;
//            break;
//        }
//    }
//    
//    if(shape2 == nullptr) {
//        return;
//    }
//    
//    
//    
//    QStringList types;
//    types << "Point Correspondences" << "Face Correspondences";
//    
//    QString type = QInputDialog::getItem(this, tr("Which Correspondences?"), tr("Correspondences"), types, 0, true, &ok);
//    
//    if(!ok) {
//        return;
//    }
//    
//    double percentage = QInputDialog::getDouble(
//                                          this,
//                                          tr("Percentage"),
//                                          tr("Percentage of correspondences"),
//                                          0,
//                                          0.001,
//                                          100.0,
//                                          1,
//                                          &ok
//                                          );
//    
//    if(!ok) {
//        return;
//    }
//    
//    int step = ceil(100.0 / percentage);
//    
//    if(type == types.value(0)) {
//        for(int i = 0; i < min(shape1->getPolyData()->GetNumberOfPoints(), shape2->getPolyData()->GetNumberOfPoints()); i+=step) {
//            PointCorrespondenceData* data = new PointCorrespondenceData(lastInsertCorresondenceID_);
//            data->getShapeIds().push_back(shape1->getId());
//            data->getShapeIds().push_back(shape2->getId());
//            
//            data->getCorrespondingIds().push_back(i);
//            data->getCorrespondingIds().push_back(i);
//            
//            lastInsertCorresondenceID_++;
//            
//            pointCorrespondenceData_.add(data, false);
//            
//            // fire event for correspondenceTabs
//            for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
//                it->second->onCorrespondenceAdd(data);
//            }
//        }
//    } else {
//        for(int i = 0; i < min(shape1->getPolyData()->GetNumberOfCells(), shape2->getPolyData()->GetNumberOfCells()); i+=step) {
//            FaceCorrespondenceData* data = new FaceCorrespondenceData(lastInsertCorresondenceID_);
//            data->getShapeIds().push_back(shape1->getId());
//            data->getShapeIds().push_back(shape2->getId());
//            
//            data->getCorrespondingIds().push_back(i);
//            data->getCorrespondingIds().push_back(i);
//            
//            lastInsertCorresondenceID_++;
//            
//            faceCorrespondenceData_.add(data, false);
//            
//            // fire event for correspondenceTabs
//            for(HashMap<string, qtCorrespondencesTab*>::iterator it = correspondencesTabs_.begin(); it != correspondencesTabs_.end(); it++) {
//                it->second->onCorrespondenceAdd(data);
//            }
//        }
//    }
//}


///////////////////////////////////////////////////////////////////////////////
//void ShapeAnalyzer::qtTransferCoordinateFunction(Shape* shape1) {
//    QStringList labels;
//    for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
//        if(it->second->getId() == shape1->getId())
//            continue;
//        
//        QString label = QString::number(it->second->getId());
//        label.append(QString::fromStdString(":"+it->second->getName()));
//        labels << label;
//        
//    }
//    bool ok;
//    QString chosen = QInputDialog::getItem(this,
//                                           "Choose a shape",
//                                           "Choose a shape:",
//                                           labels,
//                                           0,
//                                           true,
//                                           &ok);
//    if(!ok) {
//        return;
//    }
//    
//    
//    Shape* shape2 = nullptr;
//    vtkIdType shapeId = chosen.split(':')[0].toInt();
//    for(HashMap<vtkActor*, Shape*>::iterator it = shapesByActor_.begin(); it != shapesByActor_.end(); it++) {
//        if(it->second->getId() == shapeId) {
//            shape2 = it->second;
//            break;
//        }
//    }
//    
//    if(shape2 == nullptr) {
//        return;
//    }
//    
//    segmentations_.remove(shape1);
//    segmentations_.remove(shape2);
//    
//    
//    QStringList colorings;
//    colorings << "X-coordinate";
//    colorings << "Y-coordinate";
//    colorings << "Z-coordinate";
//
//    chosen = QInputDialog::getItem(this,
//                                           "Choose a coloring",
//                                           "Color shape according to",
//                                           colorings,
//                                           0,
//                                           true,
//                                           &ok);
//    if(!ok) {
//        return;
//    }
//    
//    int coordinate = colorings.indexOf(chosen);
//    
//    
//    // compute x-, y-, or z-coordinate coloring and color shape accordingly
//    ScalarPointAttribute f(shape1);
//    
//    for(int i = 0; i < shape1->getPolyData()->GetNumberOfPoints(); i++) {
//        double p[3];
//        shape1->getPolyData()->vtkDataSet::GetPoint(i, p);
//        f.getScalars()->SetValue(i, p[coordinate]);
//    }
//    shape1->colorPointsScalars(f.getScalars());
//    
//    // initialize lists of corresponding contraints on both shapes. Ordering represents correspondence of contraints. I.e. c1[5] on shape1 corresponds to c2[5] on shape2.
//    vector<ScalarPointAttribute> c1; // corresponds to contraints on shape1
//    vector<ScalarPointAttribute> c2;
//    
//    
//    // compute landmark matches using all available correspondences between shape1 and shape2 and geodesic metric
//    Metric* m1;
//    m1 = Factory<Metric>::getInstance()->create("geodesic");
//    m1->initialize(shape1);
//    
//    Metric* m2;
//    m2 = Factory<Metric>::getInstance()->create("geodesic");
//    m2->initialize(shape2);
//    
//    for(HashMap<PointCorrespondenceData*, bool>::iterator it = pointCorrespondenceData_.begin(); it != pointCorrespondenceData_.end(); it++) {
//        PointCorrespondenceData* corr = it->first;
//        
//        for(int i = 0; i < corr->getShapeIds().size(); i++) {
//            if(corr->getShapeIds()[i] == shape1->getId()) {
//                
//                
//                ScalarPointAttribute distances(shape1);
//                m1->getAllDistances(distances, corr->getCorrespondingIds()[i]);
//                c1.push_back(distances);
//                
//            }
//            
//            if(corr->getShapeIds()[i] == shape2->getId()) {
//                
//                ScalarPointAttribute distances(shape2);
//                m2->getAllDistances(distances, corr->getCorrespondingIds()[i]);
//                c2.push_back(distances);
//                
//            }
//        }
//    }
//    
//    delete m1;
//    delete m2;
//    
//    LaplaceBeltramiOperator* laplacian1 = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
//    laplacian1->initialize(shape1, 100);
//    LaplaceBeltramiOperator* laplacian2 = Factory<LaplaceBeltramiOperator>::getInstance()->create("fem");
//    laplacian2->initialize(shape2, 100);
//    
//    
//    // compute 200-dimensional wave kernel discriptor on both shapes
//    LaplaceBeltramiSignature* wks1 = Factory<LaplaceBeltramiSignature>::getInstance()->create("wks");
//    wks1->setLaplacian(laplacian1);
//    wks1->initialize(shape1, 200);
//    
//    
//    LaplaceBeltramiSignature* wks2 = Factory<LaplaceBeltramiSignature>::getInstance()->create("wks");
//    wks2->setLaplacian(laplacian2);
//    wks2->initialize(shape2, 200);
//    
//    // use first 125 components of wave kernel signature as additional constraints. Truncate rest because wave kernel seems to be inaccurate in higher dimensions
//    for(int i = 0; i < 200; i++) {
//        ScalarPointAttribute wksi1(shape1);
//        wks1->getComponent(i, wksi1);
//        c1.push_back(wksi1);
//        
//        ScalarPointAttribute wksi2(shape2);
//        wks2->getComponent(i, wksi2);
//        c2.push_back(wksi2);
//    }
//    
//    delete wks1;
//    delete wks2;
//    
//    // compute correspondence matrix C
//    FunctionalMaps functionalMaps(*shape1, *shape2, laplacian1, laplacian2, c1, c2, 100);
//    
//    // transfer the coordinate function
//    ScalarPointAttribute Tf(shape2);
//    functionalMaps.transferFunction(f, Tf);
//    
//    delete laplacian1;
//    delete laplacian2;
//    
//    // color 2nd shape
//    shape2->colorPointsScalars(Tf.getScalars());
//}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuCorrepondences(const QPoint &pos) {
    QMenu menu;
    QAction* hideAction   = menu.addAction("Hide");
    QAction* deleteAction   = menu.addAction("Delete");
    // ...
    
    QAction* selectedItem = menu.exec(pos);
    if (selectedItem == deleteAction) { // deletes correspondence data
        deleteCorrespondence(this->listCorrespondences->currentRow());
    } else if (selectedItem == hideAction) { // correspondence data is stored, vtkActor is deleted
        hideCorrespondence(this->listCorrespondences->currentRow());
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtParseContextMenuItems(QMenu* menu, HashMap<QAction*, string>& customActions) {
    // get list of all menu item paths (Home>>foo>>bar>>action)
    const unordered_map<string, string>& paths = CustomContextMenuItemFactory::getInstance()->getLabels();
    
    // menus in the menu tree indexed by their unique path.
    HashMap<string, QMenu*> menus;
    
    // for each path element A (...>>A>>...) create a unique menu in the menu tree
    for(auto entry : paths) {
        QString path(entry.second.c_str());
        QStringList list = path.split(">>");
        int i = 0;
        
        QString currentPath = "";
        QString parent = "";
        
        // iterate over all path elements except the leaf and create a menu in the hierarchy
        for(; i < list.size() - 1; i++) {
            currentPath.append((i != 0 ? ">>" : "")).append(list[i]);
            
            QMenu* currentMenu;
            if(menus.containsKey(currentPath.toStdString())) {
                currentMenu = menus[currentPath.toStdString()];
            } else {
                currentMenu = new QMenu(list[i]);
                if(i == 0) {
                    menu->addMenu(currentMenu);
                }
                menus.insert(currentPath.toStdString(), currentMenu);
                if(menus.containsKey(parent.toStdString())) {
                    menus[parent.toStdString()]->addMenu(currentMenu);
                }
            }
            
            parent = currentPath;
        }
        
        // create a action for the leaf (last element of path) and return it via customActions (here the actions are indexed by the unique identifier of the ContextMenuItem class)
        if(i > 0) {
            QAction* action = menus[parent.toStdString()]->addAction(list[i]);
            customActions.insert(action, entry.first);
        } else {
            QAction* action = menu->addAction(list[i]);
            customActions.insert(action, entry.first);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtShowContextMenuShapes(const QPoint &pos, vtkIdType pointId, vtkIdType faceId) {
    QMenu myMenu;

    QAction* opacityAction  = myMenu.addAction("Set Opacity");
    QAction* clearAction    = myMenu.addAction("Clear Coloring");
    myMenu.addSeparator();
    QAction* renameAction   = myMenu.addAction("Rename");
    QAction* deleteAction   = myMenu.addAction("Delete");
    QAction* exportAction   = myMenu.addAction("Export Shape");
    myMenu.addSeparator();

    //create custom menu items out of the list of custom context menu items registered in the CustomContextMenuItemFactory
    HashMap<QAction*, string> customActions;
    qtParseContextMenuItems(&myMenu, customActions);

    Shape* currentShape;
    CustomListWidgetItem<Shape> *item = (CustomListWidgetItem<Shape> *) this->listShapes->currentItem();
    currentShape = item->getItem();
    
    QAction* selectedItem = myMenu.exec(pos);
    if (selectedItem == deleteAction) {
        deleteShape(this->listShapes->currentRow());
    } else if (selectedItem == clearAction) {
        ((CustomListWidgetItem<Shape>*) this->listShapes->currentItem())->getItem()->clearColoring();
    } else if (selectedItem == renameAction) {
        qtInputDialogRenameShape((CustomListWidgetItem<Shape>*) this->listShapes->currentItem());
    } else if (selectedItem == exportAction) {
        qtExportShapeDialog(currentShape);
    } else if (selectedItem == opacityAction) {
        qtInputDialogOpacity(currentShape);
    } else {
        if(customActions.containsKey(selectedItem)) {
            shared_ptr<CustomContextMenuItem> menuItem = CustomContextMenuItemFactory::getInstance()->create(customActions[selectedItem]);
            menuItem->onClick(currentShape, pointId, faceId, this);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtCreateMenuCustomTabs() {
    const unordered_map<string, string>& paths = CustomTabFactory::getInstance()->getLabels();
    
    // for each key + path pair create entry in view menu and connect it to slotViewCustomTab.
    for(auto entry : paths) {
        QString path(entry.second.c_str());
        QStringList list = path.split(">>");
        QAction* action;
        if(list.count() < 2) {
            action = this->menuView->addAction(path);
        } else {
            action = this->menuView->addAction(list[1]);
        }
        action->setCheckable(true);
        action->setChecked(false);
        
        // remember which action is responsible for which CustomTab class
        viewCustomTabActions_.insert(action, entry.first);
        
        connect(action, SIGNAL(toggled(bool)), this, SLOT(slotViewCustomTab(bool)));
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::qtUpdateLabelVisibleCorrespondences() {
    int numberOfVisibleCorrespondences = 0;
    int numberOfAllCorrespondences = 0;
    if(this->actionDisplayPointCorrespondences->isChecked()) {
        numberOfAllCorrespondences = pointCorrespondenceData_.size();
        numberOfVisibleCorrespondences = pointCorrespondencesByActor_.size();
    } else {
        numberOfAllCorrespondences = faceCorrespondenceData_.size();
        numberOfVisibleCorrespondences = faceCorrespondencesByActor_.size();
    }
    
    double ratio = 100.0;
    if(numberOfAllCorrespondences > 0) {
        ratio = ( (double) numberOfVisibleCorrespondences / (double) numberOfAllCorrespondences ) * 100.0;
    }
    
    stringstream ss;
    ss << "Visible: "<<numberOfVisibleCorrespondences<<" out of "<<numberOfAllCorrespondences<<" ("<< ratio <<" %)";
    
    this->labelNumberOfVisibleCorrespondences->setText(QString(ss.str().c_str()));
}


///////////////////////////////////////////////////////////////////////////////
CustomListWidgetItem<Correspondence>* ShapeAnalyzer::qtAddListCorrespondencesItem(Correspondence* correspondence) {
    string label = "Correspondence ";
    label.append(to_string(correspondence->getData()->getId()));
    CustomListWidgetItem<Correspondence>* item = new CustomListWidgetItem<Correspondence>(
                                                                                  QString::fromStdString(label),
                                                                                  correspondence);
    item->setToolTip(correspondence->getData()->toString().c_str());
    this->listCorrespondences->addItem(item);
    return item;
}

///////////////////////////////////////////////////////////////////////////////
// QT slots
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotViewCustomTab(bool visible) {
    // obtain sender action and query key of selected CustomTab
    string key = viewCustomTabActions_[(QAction*) sender()];
    if(visible) {
        CustomTab* tab = CustomTabFactory::getInstance()->create(key, shapesByActor_, pointCorrespondenceData_, faceCorrespondenceData_, this);
        
        QString path(CustomTabFactory::getInstance()->getLabels().at(key).c_str());
        QStringList list = path.split(">>");

        if((list.count() > 1 && list[0] == "Shapes") || list.count() < 2) {
            int i = this->tabWidgetShapes->addTab(dynamic_cast<QWidget*>(tab), (list.count() > 1 ? list[1] : path));
            this->tabWidgetShapes->setCurrentIndex(i);
        } else {
            int i = this->tabWidgetCorrespondences->addTab(dynamic_cast<QWidget*>(tab), list[1]);
            this->tabWidgetCorrespondences->setCurrentIndex(i);
        }

        customTabs_.insert(key, tab);
    } else {
        delete customTabs_[key];
        customTabs_.remove(key);
    }
}


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
void ShapeAnalyzer::slotHideVisibleCorrespondences() {
    hideCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSampleCorrespondences() {
    sampleCorrespondences(this->spinBoxNumberOfSamples->value());
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotClearAllCorrespondences() {
    clearCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSetCorrespondenceType() {
    // remove all current face/point correspondences from renderer
    // then add visible point/face correspondences to renderer
    if(this->actionDisplayFaceCorrespondences->isChecked()) {
        this->labelTypeCorrespondences->setText("Type: Face Correspondences");
        if(!actionAddFaceCorrespondences->isChecked() && actionAddPointCorrespondences->isChecked()) {
            actionAddFaceCorrespondences->setChecked(true);
            actionAddFaceCorrespondences->trigger();
        }
        
        //TODO remove this
        if(tabWidgetCorrespondences->tabText(tabWidgetCorrespondences->currentIndex()) != "All Face Correspondences") {
            tabWidgetCorrespondences->setCurrentIndex(0);
        }
        
        // current picker is deleted
        delete correspondencePicker_;
        correspondencePicker_ = new FaceCorrespondencePicker(renderer_, lastInsertCorresondenceID_);
        

        // vtk: remove all point correspondences from renderer
        for(int i = listCorrespondences->count()-1; i > -1; i--) {
            //get correspondence
            CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            correspondence->removeFromRenderer();
        }
        // clear list
        this->listCorrespondences->clear();
        
        
        // add face correspondences from before
        for (auto it = faceCorrespondencesByActor_.begin(); it != faceCorrespondencesByActor_.end(); it++) {
            // add to list
            qtAddListCorrespondencesItem(it->second);
            
            // add to renderer
            it->second->addToRenderer();
            
        }

        if(faceCorrespondencesByActor_.size() > 0)
            listCorrespondences->setCurrentRow(0);
        
    } else {
        this->labelTypeCorrespondences->setText("Type: Point Correspondences");
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
            CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
            Correspondence *correspondence = item->getItem();
            
            correspondence->removeFromRenderer();
        }
        // clear list
        this->listCorrespondences->clear();
        
        
        // add point correspondences from before
        for (auto it = pointCorrespondencesByActor_.begin(); it != pointCorrespondencesByActor_.end(); it++) {
            // add to list
            qtAddListCorrespondencesItem(it->second);
            
            // add to renderer
            it->second->addToRenderer();
            
        }
        
        if(pointCorrespondencesByActor_.size() > 0)
            listCorrespondences->setCurrentRow(0);
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
    
    qtUpdateLabelVisibleCorrespondences();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotShowScalarBar(bool checked) {
    if(checked) {
        scalarBar_->SetVisibility(1);
        scalarBar_->Modified();
        this->qvtkWidget->GetRenderWindow()->Render();
    } else {
        scalarBar_->SetVisibility(0);
        scalarBar_->Modified();
        this->qvtkWidget->GetRenderWindow()->Render();
    }
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
void ShapeAnalyzer::slotOpenScene() {
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Scene"),
                                                    tr(""),
                                                    tr("Files (*.bin *.txt)"));
    
    if(filename.isEmpty())
        return;
    
    // for some strange reasen scalar bar has to be turned off before shapes are loaded otherwise application will crash.
    // This is probably a bug of VTK.
    scalarBar_->SetVisibility(0);
    scalarBar_->Modified();
    qvtkWidget->GetRenderWindow()->Render();
    clear();
    
    vector<Shape*> shapes;
    
    HashMap<PointCorrespondenceData*, PointCorrespondence*> pointCorrespondences;
    HashMap<FaceCorrespondenceData*, FaceCorrespondence*> faceCorrespondences;
    
    if(filename.endsWith(tr(".bin"))) {
        SceneWriterReader::importSceneBinary(filename.toStdString(), renderer_, lastInsertShapeID_, shapes, lastInsertCorresondenceID_, pointCorrespondences, faceCorrespondences);
    } else {
        SceneWriterReader::importSceneASCII(filename.toStdString(), renderer_, lastInsertShapeID_, shapes, lastInsertCorresondenceID_, pointCorrespondences, faceCorrespondences);
    }
    
    for(int i = 0; i < shapes.size(); i++) {
        addShape(shapes[i]);
    }
    
    for(HashMap<PointCorrespondenceData*, PointCorrespondence*>::iterator i = pointCorrespondences.begin(); i != pointCorrespondences.end(); i++) {

        
        if(i->second != nullptr) {
            pointCorrespondenceData_.insert(i->first, true);
            pointCorrespondencesByActor_.insert(i->second->getLinesActor(), i->second);
            
            // add correspondence to qt list widget
            if(actionDisplayPointCorrespondences->isChecked()) {
                i->second->addToRenderer();
                
                qtAddListCorrespondencesItem(i->second);
            }
        } else {
            pointCorrespondenceData_.insert(i->first, false);
        }
    }

    for(HashMap<FaceCorrespondenceData*, FaceCorrespondence*>::iterator i = faceCorrespondences.begin(); i != faceCorrespondences.end(); i++) {
        
        
        if(i->second != nullptr) {
            faceCorrespondenceData_.insert(i->first, true);
            faceCorrespondencesByActor_.insert(i->second->getLinesActor(), i->second);
            
            // add correspondence to qt list widget
            if(actionDisplayFaceCorrespondences->isChecked()) {
                i->second->addToRenderer();
                
                qtAddListCorrespondencesItem(i->second);
            }
        } else {
            faceCorrespondenceData_.insert(i->first, false);
        }
    }
    
    renderer_->ResetCamera();
    
    // Turn on scalarbar again.
    scalarBar_->SetVisibility(actionShowScalarBar->isChecked());
    scalarBar_->Modified();
    qvtkWidget->GetRenderWindow()->Render();
    
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotImportShape() {
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    
    QString filename = QFileDialog::getOpenFileName(this, tr("Import Shape"),
                                                    tr(""),
                                                    tr("Files (*.off *.vert *.ply *.obj)"));
    
    if(filename.isEmpty())
        return;
    
    if(filename.endsWith(tr(".off"))) {
        // read .off file
        vtkSmartPointer<vtkOFFReader> reader = vtkSmartPointer<vtkOFFReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        importShape(reader, filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1).toStdString());
    } else if(filename.endsWith(tr(".vert"))) {
        // read .tri .vert files
        vtkSmartPointer<vtkToscaReader> reader = vtkSmartPointer<vtkToscaReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        importShape(reader, filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1).toStdString());
    } else if(filename.endsWith(".ply")) {
        // read .ply file
        vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        importShape(reader, filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1).toStdString());
    } else {
        // read .obj file
        vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        importShape(reader, filename.mid(filename.lastIndexOf('/')+1, filename.lastIndexOf('.')-filename.lastIndexOf('/')-1).toStdString());
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotImportCorrespondences() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    tr(""),
                                                    tr("Files (*.txt *.bin)"));
    
    if(filename.isEmpty())
        return;
    
    SceneWriterReader reader = SceneWriterReader();
    
    vector<Shape*> shapes;
    shapesByActor_.getValues(shapes);
    
    ShapeComparator comp;
    // sort shapes to guarantee some ordering (in this case orderd by ID) which is not provided by unordered_map
    std::sort(shapes.begin(), shapes.end(), comp);
    
    vector<CorrespondenceData*> pointCorrespondences;
    vector<CorrespondenceData*> faceCorrespondences;
    
    if(filename.endsWith(".txt")) {
        reader.importCorrespondencesASCII(filename.toStdString(), lastInsertCorresondenceID_, pointCorrespondences, faceCorrespondences, shapes, this);

    } else {
        reader.importCorrespondencesBinary(filename.toStdString(), lastInsertCorresondenceID_, pointCorrespondences, faceCorrespondences, shapes, this);
    }
    
    
    // insert point correspondences
    addCorrespondences(pointCorrespondences);
    
    // insert face correspondences and fire corresponding events if vector not empty
    addCorrespondences(faceCorrespondences);
    
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSaveScene() {
    QStringList formats;
    formats << "Binary" << "Text (ASCII)";
    
    bool ok;
    QString format = QInputDialog::getItem(this, "Format", "Choose file format.", formats, 0, true, &ok);
    
    if(!ok) {
        return;
    }
    
    vector<Shape*> shapes;
    shapesByActor_.getValues(shapes);
    
    ShapeComparator comp;
    // sort shapes to guarantee some ordering (in this case orderd by ID) which is not provided by unordered_map
    std::sort(shapes.begin(), shapes.end(), comp);
    
    HashMap<PointCorrespondenceData*, PointCorrespondence*> pointCorrespondences;
    HashMap<FaceCorrespondenceData*, FaceCorrespondence*> faceCorrespondences;
    
    for(HashMap<PointCorrespondenceData*, bool>::iterator i = pointCorrespondenceData_.begin(); i != pointCorrespondenceData_.end(); i++) {
        if(i->second) {
            for(HashMap<vtkActor*, PointCorrespondence*>::iterator j = pointCorrespondencesByActor_.begin(); j != pointCorrespondencesByActor_.end(); j++) {
                if(j->second->getData() == i->first) {
                    pointCorrespondences.insert(i->first, j->second);
                    break;
                }
            }
        } else {
            pointCorrespondences.insert(i->first, nullptr);
        }
    }

    for(HashMap<FaceCorrespondenceData*, bool>::iterator i = faceCorrespondenceData_.begin(); i != faceCorrespondenceData_.end(); i++) {
        if(i->second) {
            for(HashMap<vtkActor*, FaceCorrespondence*>::iterator j = faceCorrespondencesByActor_.begin(); j != faceCorrespondencesByActor_.end(); j++) {
                if(j->second->getData() == i->first) {
                    faceCorrespondences.insert(i->first, j->second);
                    break;
                }
            }
        } else {
            faceCorrespondences.insert(i->first, nullptr);
        }
    }
    
    if(format == formats.value(0)) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Binary Scene Files (*.bin)"));
        if(filename.isEmpty()) {
            return;
        }
        SceneWriterReader::exportSceneBinary(filename.toStdString(), shapes, lastInsertShapeID_, pointCorrespondences, faceCorrespondences, lastInsertCorresondenceID_);
    } else {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Text (ASCII) Scene Files (*.txt)"));
        if(filename.isEmpty()) {
            return;
        }
        SceneWriterReader::exportSceneASCII(filename.toStdString(), shapes, lastInsertShapeID_, pointCorrespondences, faceCorrespondences, lastInsertCorresondenceID_);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotExportCorrespondences() {
    SceneWriterReader writer = SceneWriterReader();
    
    QStringList types;
    types << "Point Correspondences" << "Face Correspondences";
    bool ok; // stores if user pressed ok button
    QString save = QInputDialog::getItem(this, tr("Which Correspondences?"), tr("Correspondences"), types, 0, true, &ok);
    
    if(!ok) {
        return;
    }
    
    QStringList formats;
    formats << "Binary" << "Text (ASCII)";
    
    
    QString format = QInputDialog::getItem(this, "Format", "Choose file format.", formats, 0, true, &ok);
    
    if(!ok) {
        return;
    }

    
    //sort shapes since result of HashMap.getValues is unordered
    vector<Shape*> shapes;
    shapesByActor_.getValues(shapes);
    
    ShapeComparator comp;
    
    std::sort(shapes.begin(), shapes.end(), comp);
    
    if (save == types.value(0) && pointCorrespondenceData_.size() > 0) {
        if(format == formats.value(0)) {
            QString filename = QFileDialog::getSaveFileName(
                                                            this,
                                                            tr("Save file"),
                                                            tr(""),
                                                            tr("Binary Point Correspondence Files (*.bin)")
                                                            );
            if (!filename.isEmpty()) {
                writer.exportPointCorrespondencesBinary(pointCorrespondenceData_, shapes, filename.toStdString());
            }
        } else {
            QString filename = QFileDialog::getSaveFileName(
                                                            this,
                                                            tr("Save file"),
                                                            tr(""),
                                                            tr("ASCII Point Correspondence Files (*.txt)")
                                                            );
            if (!filename.isEmpty()) {
                writer.exportPointCorrespondencesASCII(pointCorrespondenceData_, shapes, filename.toStdString());
            }
        }
    }
    
    
    if (save == types.value(1) && faceCorrespondenceData_.size() > 0) {
        if(format == formats.value(0)) {
            QString filename = QFileDialog::getSaveFileName(
                                                            this,
                                                            tr("Save file"),
                                                            tr(""),
                                                            tr("Binary Face Correspondence Files (*.bin)")
                                                            );
            if (!filename.isEmpty()) {
                writer.exportFaceCorrespondencesBinary(faceCorrespondenceData_, shapes, filename.toStdString());
            }
        } else {
            QString filename = QFileDialog::getSaveFileName(
                                                            this,
                                                            tr("Save file"),
                                                            tr(""),
                                                            tr("ASCII Face Correspondence Files (*.txt)")
                                                            );
            if (!filename.isEmpty()) {
                writer.exportFaceCorrespondencesASCII(faceCorrespondenceData_, shapes, filename.toStdString());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotSaveScreenshot() {
    QDialog* dialog = new QDialog(this, 0);
    Ui_SaveScreenshotDialog ui;
    ui.setupUi(dialog);

    if(dialog->exec() == QDialog::Rejected) {
        return;
    }
    
    
    vtkSmartPointer<vtkGL2PSExporter> exporter = vtkSmartPointer<vtkGL2PSExporter>::New();
    
    
    QString filename;
    if(ui.comboBoxFileFormat->currentText() == "PNG") {
        filename = QFileDialog::getSaveFileName(this, tr("Save scene as image"), tr(""), tr("PNG (*.png)"));
    } else if(ui.comboBoxFileFormat->currentText() == "PDF") {
        filename = QFileDialog::getSaveFileName(this, tr("Save scene as image"), tr(""), tr("PDF (*.pdf)"));
        exporter->SetFileFormatToPDF();
    } else if(ui.comboBoxFileFormat->currentText() == "SVG") {
        filename = QFileDialog::getSaveFileName(this, tr("Save scene as image"), tr(""), tr("SVG (*.svg)"));
        exporter->SetFileFormatToSVG();
    } else if(ui.comboBoxFileFormat->currentText() == "PS") {
        filename = QFileDialog::getSaveFileName(this, tr("Save scene as image"), tr(""), tr("PS (*.ps)"));
        exporter->SetFileFormatToPS();
    } else if(ui.comboBoxFileFormat->currentText() == "EPS") {
        filename = QFileDialog::getSaveFileName(this, tr("Save scene as image"), tr(""), tr("EPS (*.eps)"));
        exporter->SetFileFormatToEPS();
    }
    
    if(filename.isEmpty())
        return;
    
    vtkSmartPointer<vtkCamera> oldCamera = renderer_->GetActiveCamera();
    
    // if checked, rerender to fit the whole scene
    if(ui.checkBoxResetCamera->isChecked()){
        vtkSmartPointer<vtkCamera> newCamera = vtkSmartPointer<vtkCamera>::New();
        newCamera->DeepCopy(oldCamera);
        
        renderer_->SetActiveCamera(newCamera);
        renderer_->ResetCamera();
        renderer_->GetRenderWindow()->Render();
    }
    
    
    if(ui.comboBoxFileFormat->currentText() != "PNG") {
        exporter->SetRenderWindow(renderer_->GetRenderWindow());
        exporter->SetFilePrefix(filename.split('.')[0].toStdString().c_str());
        exporter->SetCompress(0);
        exporter->Write();
    } else {
        // save in file
        vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(renderer_->GetRenderWindow());
        // transparency settings
        if (!(ui.checkBoxTransparency->isChecked()))
            windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
        windowToImageFilter->ShouldRerenderOff();
        windowToImageFilter->Update();
        
        vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
        writer->SetFileName(filename.toStdString().c_str());
        writer->SetInputConnection(windowToImageFilter->GetOutputPort());
        writer->Write();
    }
    
    
    
    // if checked, rerender to fit the whole scene
    if(ui.checkBoxResetCamera->isChecked()){
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
void ShapeAnalyzer::slotShowContextMenuShapes(const QPoint& pos) {
    int currentRow = this->listShapes->currentRow();
    
    // menu appears only if item was selected
    if(currentRow >= 0) {
        
        QPoint globalPos = this->listShapes->mapToGlobal(pos);

        qtShowContextMenuShapes(globalPos, -1, -1);
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::slotToggleBoxWidget() {
    if(listShapes->count() > 0) {
        Shape* selectedShape = ((CustomListWidgetItem<Shape>*) listShapes->currentItem())->getItem();
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
        Shape* currentShape = ((CustomListWidgetItem<Shape>*) current)->getItem();
        
        scalarBar_->SetLookupTable(currentShape->getMapper()->GetLookupTable());
        scalarBar_->SetTitle(currentShape->getName().c_str());
        scalarBar_->Modified();
        qvtkWidget->GetRenderWindow()->Render();
        
        if(this->actionTransformShapes->isChecked()) {
            currentShape->getBoxWidget()->On();
            
            if(previous != nullptr) {
                ((CustomListWidgetItem<Shape>*) previous)->getItem()->getBoxWidget()->Off();
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
        Correspondence* currentCorrespondence = ((CustomListWidgetItem<Correspondence>*) current)->getItem();
        
        // set current correspondence red
        currentCorrespondence->setSelected(true);
        
        // update
        this->qvtkWidget->GetRenderWindow()->Render();
    }
    
    //if there exists a previous selection set previous correspondence to green
    if(previous != nullptr) {
        ((CustomListWidgetItem<Correspondence>*) previous)->getItem()->setSelected(false);
        
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
    
    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
    lookupTable->SetTableRange(1, 1);
    lookupTable->SetHueRange(0.667, 0.0);
    lookupTable->Build();
    
    scalarBar_ = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar_->SetLookupTable(lookupTable);
    scalarBar_->SetTitle(" ");
    scalarBar_->SetWidth(0.1);
    scalarBar_->SetHeight(0.9);
    scalarBar_->SetPosition(0.9, 0);
    scalarBar_->SetNumberOfLabels(15);
    scalarBar_->SetVisibility(0);
    
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    renderer_->AddActor2D(scalarBar_);
    
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer_);
    
    vtkSmartPointer<ShapeAnalyzerInteractorStyle> style =
    vtkSmartPointer<ShapeAnalyzerInteractorStyle>::New();
    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle( style );
    
    this->correspondencePicker_ = new PointCorrespondencePicker(renderer_, lastInsertCorresondenceID_);
    
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::importShape(vtkPolyDataAlgorithm* reader, string name) {
    // the following will filter the shape for certain properties
    vtkAlgorithmOutput* output;
    output = reader->GetOutputPort();
    
    // dialog for options
    QDialog* dialog = new QDialog(this, 0);
    Ui_OpenShape ui;
    ui.setupUi(dialog);
    
    if(dialog->exec() == QDialog::Rejected) {
        return;
    }
    
    
    vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    vtkSmartPointer<vtkCleanPolyData> cleanPolyData = vtkSmartPointer<vtkCleanPolyData>::New();
    
    // filter to triangulate shape
    if(ui.checkTriangulation->isChecked()) {
        //make sure that all faces are triangles
        triangleFilter->SetInputConnection(output);
        triangleFilter->Update();
        output = triangleFilter->GetOutputPort();
    }

    //Remove all isolated points.
    if(ui.checkDegeneratedElements->isChecked()) {
        cleanPolyData->SetInputConnection(output);
        cleanPolyData->Update();
        output = cleanPolyData->GetOutputPort();
    }

    
    //If shape is not connected (This only happens with bad shape data). Find largest connected region and extract it.
    if(ui.checkLargestComponent->isChecked()) {
        vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
        vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
        connectivityFilter->SetInputConnection(output);
        connectivityFilter->SetExtractionModeToLargestRegion();
        connectivityFilter->Update();
        output = connectivityFilter->GetOutputPort();
        
        // get vtk actor and add to renderer_
        vtkSmartPointer<vtkPolyDataReader> polyDataReader = (vtkPolyDataReader*) output->GetProducer();
        Shape* shape = new Shape(lastInsertShapeID_, name, polyDataReader->GetOutput(), renderer_);
        shape->initialize();
        addShape(shape);
        
    } else {
        // extract all regions in different shapes
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
            
            // get vtk actor and add to renderer_
            vtkSmartPointer<vtkPolyDataReader> polyDataReader = (vtkPolyDataReader*) connectivityFilter->GetOutputPort()->GetProducer();
            Shape* shape = new Shape(lastInsertShapeID_, name, polyDataReader->GetOutput(), renderer_);
            shape->initialize();
            addShape(shape);
        }
    }
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
            vtkShapeClicked(shape, picker->GetPointId(), picker->GetCellId(), globalPos, vtkEvent, command);
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
                pickCorrespondence();
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
        if(((CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i))->getItem() == correspondence) {
            listCorrespondences->setCurrentRow(i);
            if(vtkEvent == vtkCommand::RightButtonPressEvent) {
                qtShowContextMenuCorrepondences(pos);
            }
            break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkShapeClicked(Shape* shape, vtkIdType pointId, vtkIdType faceId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command) {
    if((this->actionAddFaceCorrespondences->isChecked() || this->actionAddPointCorrespondences->isChecked())&& vtkEvent == vtkCommand::LeftButtonPressEvent) {
        
        int result = correspondencePicker_->addShape(shape, this->actionAddFaceCorrespondences->isChecked() ? faceId : pointId);
        if(result == 1)
            pickerCounter_++;
        else if(result == -1)
            pickerCounter_ = 1;
        else
            ;
        
        // all shapes are in the current correspondence
        if(shapesByActor_.size() > 1 && pickerCounter_ == shapesByActor_.size()) {
            pickCorrespondence();
        }
        
        
    } else {
        //select item in list if clicked on shape
        for(int i = 0; i < listShapes->count(); i++) {
            if(((CustomListWidgetItem<Shape>*) listShapes->item(i))->getItem() == shape) {
                listShapes->setCurrentRow(i);
                if(vtkEvent == vtkCommand::RightButtonPressEvent && !this->actionTransformShapes->isChecked()) {
                    command->AbortFlagOn();
                    qtShowContextMenuShapes(pos, pointId, faceId);
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
    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
    lookupTable->SetTableRange(1.0, 1.0);
    lookupTable->SetHueRange(0.667, 0.0);
    lookupTable->Build();
    scalarBar_->SetLookupTable(lookupTable);
    scalarBar_->SetTitle(" ");
    scalarBar_->Modified();


    // fire event for customTabs
    for(auto entry : customTabs_) {
        entry.second->onClear();
    }
    
    // qt
    listShapes->disconnect();
    listCorrespondences->disconnect();
    
    //delete all items from the list. Remove displayed correspondences from renderer
    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
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
        CustomListWidgetItem<Shape> *item = (CustomListWidgetItem<Shape>*) listShapes->item(i);
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
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addCorrespondences(const vector<CorrespondenceData*>& correspondences) {
    for(auto correspondence : correspondences) {
        addCorrespondence(correspondence);
    }
    
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addCorrespondence(CorrespondenceData *correspondence) {
    if(correspondence->getType() == "PointCorrespondenceData") {
        pointCorrespondenceData_.insert((PointCorrespondenceData*) correspondence, false);
    } else {
        faceCorrespondenceData_.insert((FaceCorrespondenceData*) correspondence, false);
    }
    
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::clearCorrespondences() {
    listCorrespondences->disconnect();
    
    //delete all items from the list if point correspondences are currently in the listWidget. Remove displayed correspondences from renderer
    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
        Correspondence *correspondence = item->getItem();
        
        delete item;
        correspondence->removeFromRenderer();
    }
    
    if(actionDisplayPointCorrespondences->isChecked()) {
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
    } else {
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
    }
    
    qtConnectListCorrespondences();
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
    
    
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
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::hideCorrespondences() {
    listCorrespondences->disconnect();

    for(int i = listCorrespondences->count()-1; i > -1; i--) {
        //get correspondence
        CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
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
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::hideCorrespondence(int i) {
    // qt
    listCorrespondences->disconnect();
    
    CustomListWidgetItem<Correspondence> *item = (CustomListWidgetItem<Correspondence>*) listCorrespondences->item(i);
    
    
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
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::sampleCorrespondences(unsigned int size) {
    hideCorrespondences();
    if (this->actionDisplayPointCorrespondences->isChecked()) { // point correspondence
        vector<PointCorrespondenceData*> data;
        pointCorrespondenceData_.getRandomSampleKeys(size, data);
        
        for(int i = 0; i < data.size(); i++) {
            
            PointCorrespondence* correspondence = new PointCorrespondence(renderer_, data[i], shapesByActor_);
            
            // create actor and add to vtk
            correspondence->initialize();
            correspondence->addToRenderer();
            
            pointCorrespondenceData_[data[i]] = true;
            pointCorrespondencesByActor_.insert(correspondence->getLinesActor(), correspondence);
            
            // add shape to qt list widget
            qtAddListCorrespondencesItem(correspondence);
        }
    } else {
        vector<FaceCorrespondenceData*> data;
        faceCorrespondenceData_.getRandomSampleKeys(size, data);
        
        for(int i = 0; i < data.size(); i++) {
            
            FaceCorrespondence* correspondence = new FaceCorrespondence(renderer_, data[i], shapesByActor_);
            
            // create actor and add to vtk
            correspondence->initialize();
            correspondence->addToRenderer();
            
            faceCorrespondenceData_[data[i]] = true;
            faceCorrespondencesByActor_.insert(correspondence->getLinesActor(), correspondence);
            
            // add shape to qt list widget
            qtAddListCorrespondencesItem(correspondence);
        }
    }
    
    this->qvtkWidget->GetRenderWindow()->Render();
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::pickCorrespondence() {
    Correspondence* correspondence;
    
    // adding to correspondenceByActor, vtkPart
    if(correspondencePicker_->pick(&correspondence)) {
        // add to qt
        lastInsertCorresondenceID_++;
        
        // adding to face/point data
        if (correspondence->getData()->getType() == "PointCorrespondenceData") { // point correspondence
            PointCorrespondence* pointCorrespondence = (PointCorrespondence*) correspondence;
            pointCorrespondenceData_.insert(pointCorrespondence->getData(), true);
            
            pointCorrespondencesByActor_.insert(pointCorrespondence->getLinesActor(), pointCorrespondence);
            
            // add shape to qt list widget
            qtAddListCorrespondencesItem(correspondence);
        } else { // face correspondence
            FaceCorrespondence* faceCorrespondence = (FaceCorrespondence*) correspondence;
            faceCorrespondenceData_.insert(faceCorrespondence->getData(), true);
            faceCorrespondencesByActor_.insert(faceCorrespondence->getLinesActor(), faceCorrespondence);
            
            
            // add shape to qt list widget
            qtAddListCorrespondencesItem(correspondence);
        }
    }
    
    pickerCounter_ = 0;
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteShape(int i) {
    listCorrespondences->disconnect();
    listShapes->disconnect();
    
    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
    lookupTable->SetTableRange(1.0, 1.0);
    lookupTable->SetHueRange(0.667, 0.0);
    lookupTable->Build();
    scalarBar_->SetLookupTable(lookupTable);
    scalarBar_->SetTitle(" ");
    scalarBar_->Modified();
    
    
    // qt
    CustomListWidgetItem<Shape> *item = (CustomListWidgetItem<Shape>*) this->listShapes->item(i);
    Shape* shape = item->getItem();
    

    // fire event for shapesTabs
    for(auto entry : customTabs_) {
        entry.second->onShapeDelete(shape);
    }
    
    
    correspondencePicker_->clearSelection();
    pickerCounter_ = 0;
    //first remove shape from list
    delete item;
    
    //first delete all items in the list
    //iterate through the correspondence list backwards beginning from the end of the list since delete listItem decreases list->count() and to make sure that we get all correspondences that we want to delete.
    for(int j = listCorrespondences->count()-1; j > -1; j--) {
        //get correspondence
        Correspondence *correspondence = ((CustomListWidgetItem<Correspondence>*) listCorrespondences->item(j))->getItem();
        

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
    qtUpdateLabelVisibleCorrespondences();
}


///////////////////////////////////////////////////////////////////////////////
Shape* ShapeAnalyzer::addShape(string name, vtkSmartPointer<vtkPolyData> polyData) {
    Shape* shape = new Shape(lastInsertShapeID_, name, polyData, renderer_);
    shape->initialize();
    addShape(shape);
    return shape;
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addShape(Shape* shape) {
    if(actionShowSurfaceNormals->isChecked()) {
        shape->getMapper()->SetInputData(shape->getPolyDataNormals()->GetOutput());
    }
    
    
    if(shapesByActor_.size() == 0)
        renderer_->ResetCamera();
    
    vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
    lookupTable->SetTableRange(1.0, 1.0);
    lookupTable->SetHueRange(0.667, 0.0);
    lookupTable->Build();
    
    shape->getMapper()->SetLookupTable(lookupTable);
    
    vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;
    
    shape->getBoxWidget()->AddObserver(vtkCommand::InteractionEvent, callback);

    shapesByActor_.insert(shape->getActor(), shape);
    
    // fire event for shapesTabs
    for(auto entry : customTabs_) {
        entry.second->onShapeAdd(shape);
    }
    
    // add shape to qt list widget
    lastInsertShapeID_++;
    
    CustomListWidgetItem<Shape> *item = new CustomListWidgetItem<Shape>(QString(shape->getName().c_str()), shape);
    listShapes->addItem(item);
    
    //make sure that there always is exactly one item selected if there exists at least one item
    listShapes->setCurrentRow(listShapes->count()-1);
    
    
    qvtkWidget->GetRenderWindow()->Render();
}

