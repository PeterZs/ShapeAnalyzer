#include "ShapeAnalyzer.h"


// Constructor
ShapeAnalyzer::ShapeAnalyzer() {
    this->setupUi(this);

    this->listShapes->setContextMenuPolicy(Qt::CustomContextMenu);

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
            this,                                   SLOT(showContextMenu(const QPoint&)));
    
    this->setupVTK();
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
void ShapeAnalyzer::toggleBoxWidgets() {
    if(this->radioButtonTransformActors->isChecked()) {
        for(int i = 0; i < numberOfActors; i++) {
            shapes_[i].getBoxWidget()->On();
        }
    } else {
        for(int i = 0; i < numberOfActors; i++) {
            shapes_[i].getBoxWidget()->Off();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int ShapeAnalyzer::getActorId(vtkActor *actor) {
    for(int i = 0; i < numberOfActors; i++) {
        if(shapes_[i].getActor() == actor) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::resetCamera() {
    renderer->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::clear() {
    // qt
    this->listShapes->clear();

    // delete all shapes
    for(int i = 0; i < numberOfActors; i++) {
        deleteShape(i);
    }
    
    for(std::vector<vtkSmartPointer<vtkActor> >::iterator i = lines.begin(); i != lines.end(); i++) {
        renderer->RemoveActor(*i);
    }
    lines.clear();
    correspondences.clear();
    sources.clear();
    targets.clear();
    
    this->numberOfActors = 0;
    this->actorId = -1;
    
    renderer->RemoveActor(selectedActor);
    this->qvtkWidget->GetRenderWindow()->Render();
}

void ShapeAnalyzer::deleteShape(int i) {
    printf("%d \n", i);

    // qt
    this->listShapes->takeItem(i);

    // vtk
    renderer->RemoveActor(shapes_[i].getActor());
    shapes_[i].getBoxWidget()->SetInteractor(NULL);
    shapes_[i].getBoxWidget()->SetProp3D(NULL);
    shapes_.erase(shapes_.begin() + i);

    // TODO delete correspondences with this shape
    //for(std::vector<vtkSmartPointer<vtkActor> >::iterator i = lines.begin(); i != lines.end(); i++) {
    //    renderer->RemoveActor(*i);
    //}

    this->numberOfActors--;

    // TODO not sure if this makes sense
    this->actorId = -1;

    this->qvtkWidget->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::exit() {
    qApp->exit();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief ShapeAnalyzer::showContextMenu
/// opens menu for shapes
void ShapeAnalyzer::showContextMenu(const QPoint& pos) {
    // for most widgets
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


///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::openShape() {
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.off)"));
    
    if(fileName.count() == 0)
        return;
    
    addShapeToVTK(fileName);

    // add shape to qt list widget
    std::string name = "Shape ";
    name.append(std::to_string(this->listShapes->count() + 1));
    this->listShapes->addItem(QString(name.c_str()));
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::toggleCurrent() {
    if(this->radioButtonTransformActors->isChecked()) {
        int index = this->listShapes->currentRow();

        // one might optimize that...
        for(int i = 0; i < numberOfActors; i++) {
            shapes_[i].getBoxWidget()->Off();
        }

        // if a row is selected
        if(index >= 0)
                shapes_[index].getBoxWidget()->On();
    }
}

///////////////////////////////////////////////////////////////////////////////
bool ShapeAnalyzer::eventFilter(QObject *object, QEvent *event) {
    if (object == qvtkWidget && event->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::addShapeToVTK(QString fileName) {
    Shape *shape = new Shape();
    shapes_.push_back(*shape);

    // read .off file
    vtkSmartPointer<vtkOFFReader> reader =
            vtkSmartPointer<vtkOFFReader>::New();
    reader->SetFileName(fileName.toUtf8().constData());
    shapes_[numberOfActors].setData(reader->GetOutput());

    // get vtk actor and add to renderer
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    shapes_[numberOfActors].setActor(vtkSmartPointer<vtkActor>::New());
    shapes_[numberOfActors].getActor()->SetMapper(mapper);

    renderer->AddActor(shapes_[numberOfActors].getActor());

    if(numberOfActors == 0)
        renderer->ResetCamera();

    // set up empty set of correspondence points
    shapes_[numberOfActors].setPoints(vtkSmartPointer<vtkPoints>::New());

    // set up box widget
    shapes_[numberOfActors].setBoxWidget(vtkSmartPointer<vtkBoxWidget>::New());
    shapes_[numberOfActors].getBoxWidget()->SetProp3D
                            (
                                shapes_[numberOfActors].getActor()
                            );
    shapes_[numberOfActors].getBoxWidget()->SetInteractor
                            (
                                qvtkWidget->GetRenderWindow()->GetInteractor()
                            );
    shapes_[numberOfActors].getBoxWidget()->SetPlaceFactor(1.25);

    shapes_[numberOfActors].getBoxWidget()->PlaceWidget();
    vtkSmartPointer<vtkBoxWidgetCallback> callback =
            vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;

    shapes_[numberOfActors].getBoxWidget()->AddObserver
                            (
                                vtkCommand::InteractionEvent, callback
                            );

    qvtkWidget->GetRenderWindow()->Render();

    // turn box widget on if in transform mode
    // TODO not if other shape is selected
    if(this->radioButtonTransformActors->isChecked()) {
        shapes_[numberOfActors].getBoxWidget()->On();
    } else {
        shapes_[numberOfActors].getBoxWidget()->Off();
    }

    numberOfActors++;
}

///////////////////////////////////////////////////////////////////////////////
void ShapeAnalyzer::vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command) {

    if(this->radioButtonAddCorrespondences->isChecked()) {
        cout << "clicked left" <<endl;

        vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);

        // Get the location of the click (in window coordinates)
        int* pos =interactor->GetEventPosition();
        //command->AbortFlagOn();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);

        // Pick from this location.
        picker->Pick(pos[0], pos[1], 0, renderer);

        double* worldPosition = picker->GetPickPosition();
        std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

        this->actorId = getActorId(picker->GetActor());
        if(picker->GetCellId() != -1 && this->actorId != -1) {
            std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
            << " " << worldPosition[2] << endl;

            vtkSmartPointer<vtkIdTypeArray> ids = 
                        vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());

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


            selectedMapper->SetInputData(selected);

            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(0, 1, 0);
            selectedActor->GetProperty()->SetLineWidth(3);

            selectedActor->SetUserTransform(
                            shapes_[actorId].getActor()->GetUserTransform()
                            );

            renderer->AddActor(selectedActor);


            if(sources.size() == targets.size()) {
                std::pair<int, vtkIdType> source;
                source.first = actorId;

                shapes_[actorId].getPoints()->InsertNextPoint(
                        shapes_[actorId].getData()->GetCell(
                            picker->GetCellId())->GetPoints()->GetPoint(0)
                        );
                source.second = 
                    shapes_[actorId].getPoints()->GetNumberOfPoints() - 1;

                this->sources.push_back(source);
            } else {
                std::pair<int, vtkIdType> source = sources[sources.size() - 1];
                std::pair<int, vtkIdType> target;
                target.first = actorId;

                vtkSmartPointer<vtkPoints> points = 
                                shapes_[actorId].getPoints();
                if(source.first == target.first) {
                    points->SetPoint(points->GetNumberOfPoints() - 1,
                            shapes_[actorId].getData()->GetCell(
                            picker->GetCellId())->GetPoints()->GetPoint(0)
                        );
                    return;
                }

                points->InsertNextPoint(
                    shapes_[actorId].getData()->GetCell(picker->GetCellId())->GetPoints()->GetPoint(0)
                    );
                target.second = shapes_[actorId].getPoints()->GetNumberOfPoints() - 1;

                this->targets.push_back(target);

                vtkSmartPointer<vtkLineSource> lineSource = 
                        vtkSmartPointer<vtkLineSource>::New();

                if(shapes_[source.first].getActor()->GetUserTransform() == NULL)
                    lineSource->SetPoint1(
                        shapes_[source.first].getPoints()->GetPoint(source.second)
                        );
                else
                    lineSource->SetPoint1(
                        shapes_[source.first].getActor()->GetUserTransform()->TransformPoint(shapes_[source.first].getPoints()->GetPoint(source.second))
                        );

                if(shapes_[target.first].getActor()->GetUserTransform() == NULL)
                    lineSource->SetPoint2(
                        shapes_[target.first].getPoints()->GetPoint(target.second)
                        );
                else
                    lineSource->SetPoint2(
                        shapes_[target.first].getActor()->GetUserTransform()->TransformPoint(shapes_[target.first].getPoints()->GetPoint(target.second))
                        );

                lineSource->Update();

                correspondences.push_back(lineSource);

                // Visualize
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(lineSource->GetOutputPort());

                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);
                actor->GetProperty()->SetLineWidth(1);
                actor->GetProperty()->SetColor(0, 1, 0);
                renderer->AddActor(actor);
                lines.push_back(actor);
                renderer->RemoveActor(selectedActor);
            }
        }
    }
}

