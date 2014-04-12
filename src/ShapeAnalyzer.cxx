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
    connect(this->radioButtonTransformActors,       SIGNAL(clicked()),
            this,                                   SLOT(toggleBoxWidgets()));
    connect(this->radioButtonAddCorrespondences,    SIGNAL(clicked()),
            this,                                   SLOT(toggleBoxWidgets()));
    connect(this->radioButtonTransformScene,        SIGNAL(clicked()),
            this,                                   SLOT(toggleBoxWidgets()));
    connect(this->listShapes,                       SIGNAL(itemSelectionChanged()),
            this,                                   SLOT(toggleCurrent()));
    connect(this->listShapes,                       SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(showContextMenuShapes(const QPoint&)));
    connect(this->listCorrespondences,              SIGNAL(customContextMenuRequested(const QPoint&)),
            this,                                   SLOT(showContextMenuCorrespondences(const QPoint&)));
    connect(this->radioButtonTransformActors,       SIGNAL(toggled(bool)),
            this,                                   SLOT(deleteMarkedCorrespondence()));
    connect(this->listCorrespondences,              SIGNAL(currentRowChanged (int)),
            this,                                   SLOT(setCurrentCorrespondenceColor()));
    connect(this->actionHelp,                         SIGNAL(triggered()),
            this,                                   SLOT(openHelpWindow()));
    
    this->setupVTK();
}

///////////////////////////////////////////////////////////////////////////////
// Functions handling QT 
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
bool ShapeAnalyzer::eventFilter(
    QObject     *object, 
    QEvent      *event
) {
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
void ShapeAnalyzer::deleteMarkedCorrespondence() {
    set_ = false;
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
    
    addShapeToVTK(fileName);

    numberOfActors++;

    // add shape to qt list widget
    std::string name = "Shape ";
    name.append(std::to_string(numberOfActors));
    this->listShapes->addItem(QString(name.c_str()));
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
        if (selectedItem)
        {
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
        if (selectedItem)
        {
            deleteShape(this->listShapes->currentRow());
        }

    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::toggleBoxWidgets() {
    if(this->radioButtonTransformActors->isChecked()) {
        for(vector<Shape>::iterator it = shapes_.begin(); 
            it != shapes_.end(); 
            ++it) {
            it->getBoxWidget()->On();
        }
    } else {
        for(vector<Shape>::iterator it = shapes_.begin(); 
            it != shapes_.end(); 
            ++it) {
            it->getBoxWidget()->Off();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::toggleCurrent() {
    if(this->radioButtonTransformActors->isChecked()) {
        int index = this->listShapes->currentRow();

        // one might optimize that...
        for(vector<Shape>::iterator it = shapes_.begin(); 
            it != shapes_.end(); 
            ++it) {
            it->getBoxWidget()->Off();
        }

        // if a row is selected
        if(index >= 0)
                shapes_[index].getBoxWidget()->On();
    }
}


///////////////////////////////////////////////////////////////////////////////
// Functions handling VTK
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addShapeToVTK(QString fileName) {
    int index = shapes_.size();

    Shape *shape = new Shape();
    shapes_.push_back(*shape);

    // read .off file
    vtkSmartPointer<vtkOFFReader> reader =
            vtkSmartPointer<vtkOFFReader>::New();
    reader->SetFileName(fileName.toUtf8().constData());
    shapes_[index].setData(reader->GetOutput());

    // get vtk actor and add to renderer
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    shapes_[index].setActor(vtkSmartPointer<vtkActor>::New());
    shapes_[index].getActor()->SetMapper(mapper);

    renderer->AddActor(shapes_[index].getActor());

    if(index == 0)
        renderer->ResetCamera();

    // set up box widget
    shapes_[index].setBoxWidget(vtkSmartPointer<vtkBoxWidget>::New());
    shapes_[index].getBoxWidget()->SetProp3D
                            (
                                shapes_[numberOfActors].getActor()
                            );
    shapes_[index].getBoxWidget()->SetInteractor
                            (
                                qvtkWidget->GetRenderWindow()->GetInteractor()
                            );
    shapes_[index].getBoxWidget()->SetPlaceFactor(1.25);

    shapes_[index].getBoxWidget()->PlaceWidget();
    vtkSmartPointer<vtkBoxWidgetCallback> callback =
            vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;

    shapes_[index].getBoxWidget()->AddObserver
                            (
                                vtkCommand::InteractionEvent, 
                                callback
                            );

    qvtkWidget->GetRenderWindow()->Render();

    // turn box widget on if in transform mode
    // TODO not if other shape is selected
    if(this->radioButtonTransformActors->isChecked()) {
        shapes_[index].getBoxWidget()->On();
    } else {
        shapes_[index].getBoxWidget()->Off();
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::resetCamera() {
    renderer->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::setCurrentCorrespondenceColor() {
    int currentRow = this->listCorrespondences->currentRow();

    // only change color if there is a correspondenece
    if(currentRow >= 0) {

        // set all correspondences green
        // TODO might find out which one was selected prev and only change that
        for(vector<Correspondence>::iterator it = correspondences_.begin(); 
            it != correspondences_.end();
            it++) {
                it->getActor()->GetProperty()->SetColor(0, 1, 0);
        }


        // set current correspondence red
        correspondences_[currentRow].getActor()->GetProperty()->SetColor(1, 0, 0);

        // update
        this->qvtkWidget->GetRenderWindow()->Render();
    }
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::setupVTK() {
    this->qvtkWidget->installEventFilter(this);
    
    //Connect qvtk widgets with this object
    this->connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->connections->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                               vtkCommand::LeftButtonPressEvent,
                               this,
                               SLOT(vtkClickHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)),
                               NULL, 1.0);
    
    renderer = vtkSmartPointer<vtkRenderer>::New();
    
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    
    selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    selectedActor = vtkSmartPointer<vtkActor>::New();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkClickHandler(
    vtkObject       *caller, 
    unsigned long   vtkEvent, 
    void            *clientData, 
    void            *callData, 
    vtkCommand      *command
) {

    if(this->radioButtonAddCorrespondences->isChecked()) {
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
        picker->Pick(pos[0], pos[1], 0, renderer);

        // double* worldPosition = picker->GetPickPosition();
        // std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

        this->actorId = getActorId(picker->GetActor());

        // check if pick was valid
        if(picker->GetCellId() != -1 && this->actorId != -1) {
            // std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
            // << " " << worldPosition[2] << endl;

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

            vtkSmartPointer<vtkExtractSelection> extractSelection = 
                        vtkSmartPointer<vtkExtractSelection>::New();

            extractSelection->SetInputData(0, shapes_[actorId].getData());
            extractSelection->SetInputData(1, selection);
            extractSelection->Update();

            // In selection 
            vtkSmartPointer<vtkUnstructuredGrid> selected = 
                        vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());

            // std::cout << "There are " << selected->GetNumberOfPoints()
            // << " points in the selection." << std::endl;
            // std::cout << "There are " << selected->GetNumberOfCells()
            // << " cells in the selection." << std::endl;


            // visual response for picked node
            selectedMapper->SetInputData(selected);

            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(0, 1, 0);
            selectedActor->GetProperty()->SetLineWidth(3);

            selectedActor->SetUserTransform(
                            shapes_[actorId].getActor()->GetUserTransform()
                            );

            renderer->AddActor(selectedActor);

            // depending on whether there was a selection before or not
            if(set_ == false) {
                // set source_ to current point and wait
                source_.first = shapes_[actorId];

                source_.second = vtkSmartPointer<vtkPoints>::New();
                source_.second->InsertNextPoint(shapes_[actorId].getData()->GetCell(
                             picker->GetCellId())->GetPoints()->GetPoint(0));
                set_ = true;
            } else {
                // combine source_ and current pick to a correspondence

                pair<Shape, vtkSmartPointer<vtkPoints> > target;
                target.first = shapes_[actorId];
                target.second = vtkSmartPointer<vtkPoints>::New();
                target.second->InsertNextPoint(shapes_[actorId].getData()->GetCell(
                             picker->GetCellId())->GetPoints()->GetPoint(0));

                // if picked node is on the same shape as the source, 
                // update and return
                if(source_.first.getActor() == target.first.getActor()) {
                    source_ = target;

                    return;
                }

                vtkSmartPointer<vtkLineSource> lineSource = 
                        vtkSmartPointer<vtkLineSource>::New();

                
                // set points in line source, transform if user transformation happend
                if(source_.first.getActor()->GetUserTransform() == NULL) {
                    lineSource->SetPoint1(source_.second->GetPoint(0));
                }
                 else {
                     lineSource->SetPoint1(
                         source_.first.getActor()->GetUserTransform()->TransformPoint
                             (
                                 source_.second->GetPoint(0)
                             )
                         );
                 }
                 
                 if(target.first.getActor()->GetUserTransform() == NULL) {
                    lineSource->SetPoint2(target.second->GetPoint(0));
                }
                 else {
                    lineSource->SetPoint2(
                        target.first.getActor()->GetUserTransform()->TransformPoint
                            (
                                target.second->GetPoint(0)
                            )
                        );
                 }

                lineSource->Update();

                // create correspondence
                Correspondence *correspondence = new Correspondence();
                correspondence->setCorrespondence(lineSource);
                correspondence->setShape1(source_.first);
                correspondence->setShape2(target.first);
                correspondences_.push_back(*correspondence);

                set_ = false;

                // Visualize
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(lineSource->GetOutputPort());

                vtkSmartPointer<vtkActor> actor = 
                    vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);
                actor->GetProperty()->SetLineWidth(1);
                actor->GetProperty()->SetColor(0, 1, 0);
                renderer->AddActor(actor);
                correspondences_.back().setActor(actor);
                renderer->RemoveActor(selectedActor);

                // add to qt

                // add shape to qt list widget
                numberOfCorrespondences++;

                std::string name = "Correspondence ";
                name.append(std::to_string(numberOfCorrespondences));
                this->listCorrespondences->addItem(QString(name.c_str()));
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
    this->listShapes->clear();
    this->listCorrespondences->clear();

    // vtk

    for(vector<Correspondence>::iterator it = correspondences_.begin(); 
        it != correspondences_.end(); 
        ++it) {

        renderer->RemoveActor(it->getActor());
    }

    correspondences_.clear();

    // delete all shapes
    for(vector<Shape>::iterator it = shapes_.begin(); 
        it != shapes_.end(); 
        ++it) {
            renderer->RemoveActor(it->getActor());
            it->getBoxWidget()->SetInteractor(NULL);
            it->getBoxWidget()->SetProp3D(NULL);
    }

    shapes_.clear();
    
    numberOfActors = 0;
    numberOfCorrespondences = 0;

    actorId = -1;
    
    //renderer->RemoveActor(selectedActor);
    this->qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteCorrespondence(int i) {
    // qt
    this->listCorrespondences->takeItem(i);

    // vtk
    renderer->RemoveActor(correspondences_[i].getActor());
    correspondences_.erase(correspondences_.begin() + i);

    this->qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteShape(Shape shape) {
    int i = getActorId(shape.getActor());
    deleteShape(i);
}


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::deleteShape(int i) {

    // qt
    this->listShapes->takeItem(i);

    // vtk

    // delete correspondences on i
    // TODO total ugly with the j counter, think of smth better
    int j = 0;
    for(vector<Correspondence>::iterator it = correspondences_.begin(); 
        it != correspondences_.end(); ) {
        if(shapes_[i].getActor() == it->getShape1().getActor() || 
            shapes_[i].getActor() == it->getShape2().getActor()) {

                renderer->RemoveActor(it->getActor());
                it = correspondences_.erase(it);

                // qt
                this->listCorrespondences->takeItem(j);
        } else {
            it++;
            j++;
        }
    }

    // delete shape
    renderer->RemoveActor(shapes_[i].getActor());
    shapes_[i].getBoxWidget()->SetInteractor(NULL);
    shapes_[i].getBoxWidget()->SetProp3D(NULL);
    shapes_.erase(shapes_.begin() + i);

    this->numberOfActors--;

    // TODO not sure if this makes sense
    //this->actorId = -1;

    //renderer->RemoveActor(selectedActor);

    this->qvtkWidget->GetRenderWindow()->Render();
}


///////////////////////////////////////////////////////////////////////////////
int ShapeAnalyzer::getActorId(vtkActor *actor) {
    int i = 0;
    for(vector<Shape>::iterator it = shapes_.begin();
        it != shapes_.end();
        ++it) {
        if(it->getActor() == actor) {
            return i;
        }
        i++;
    }
    return -1;
}


