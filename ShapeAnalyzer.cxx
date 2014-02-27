#include "ShapeAnalyzer.h"


// Constructor
ShapeAnalyzer::ShapeAnalyzer() {
    this->setupUi(this);

    // Set up action signals and slots
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
    connect(this->actionResetCamera, SIGNAL(triggered()), this, SLOT(resetCamera()));
    connect(this->actionClear, SIGNAL(triggered()), this, SLOT(clear()));
    connect(this->clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this->openFileButton, SIGNAL(clicked()), this, SLOT(openShape()));
    connect(this->radioButtonTransformActors, SIGNAL(clicked()), this, SLOT(toggleBoxWidgets()));
    connect(this->radioButtonAddCorrespondences, SIGNAL(clicked()), this, SLOT(toggleBoxWidgets()));
    connect(this->radioButtonTransformScene, SIGNAL(clicked()), this, SLOT(toggleBoxWidgets()));
    
    this->setupVTK();
}

void ShapeAnalyzer::setupVTK() {
    this->qvtkWidget->installEventFilter(this);
    
    //Connect qvtk widgets with object
    this->connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->connections->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
                               vtkCommand::LeftButtonPressEvent,
                               this,
                               SLOT(vtkClickHandler(vtkObject*, unsigned long, void*, void*, vtkCommand*)), NULL, 1.0);
    
    renderer = vtkSmartPointer<vtkRenderer>::New();
    
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    
    
    selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    selectedActor = vtkSmartPointer<vtkActor>::New();
}

void ShapeAnalyzer::exit() {
    qApp->exit();
}

void ShapeAnalyzer::toggleBoxWidgets() {
    if(this->radioButtonTransformActors->isChecked()) {
        for(int i = 0; i < numberOfActors; i++) {
            this->boxWidgets[i]->On();
        }
    } else {
        for(int i = 0; i < numberOfActors; i++) {
            this->boxWidgets[i]->Off();
        }
    }
}

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
            
            vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());
            
            vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);
            
            vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);
            
            vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
            
            extractSelection->SetInputData(0, this->data[actorId]);
            extractSelection->SetInputData(1, selection);
            extractSelection->Update();
            
            // In selection
            vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());
            
            std::cout << "There are " << selected->GetNumberOfPoints()
            << " points in the selection." << std::endl;
            std::cout << "There are " << selected->GetNumberOfCells()
            << " cells in the selection." << std::endl;
            
            
            selectedMapper->SetInputData(selected);
            
            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(0, 1, 0);
            selectedActor->GetProperty()->SetLineWidth(3);
            
            selectedActor->SetUserTransform(actors[actorId]->GetUserTransform());
            
            renderer->AddActor(selectedActor);


            if(sources.size() == targets.size()) {
                std::pair<int, vtkIdType> source;
                source.first = actorId;

                points[actorId]->InsertNextPoint(data[actorId]->GetCell(picker->GetCellId())->GetPoints()->GetPoint(0));
                source.second = points[actorId]->GetNumberOfPoints() - 1;

                this->sources.push_back(source);
            } else {
                std::pair<int, vtkIdType> source = sources[sources.size() - 1];
                std::pair<int, vtkIdType> target;
                target.first = actorId;
                
                if(source.first == target.first) {
                    points[actorId]->SetPoint(points[actorId]->GetNumberOfPoints() - 1, data[actorId]->GetCell(picker->GetCellId())->GetPoints()->GetPoint(0));
                    return;
                }

                points[actorId]->InsertNextPoint(data[actorId]->GetCell(picker->GetCellId())->GetPoints()->GetPoint(0));
                target.second = points[actorId]->GetNumberOfPoints() - 1;
                
                this->targets.push_back(target);
                
                vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();

                if(actors[source.first]->GetUserTransform() == NULL)
                    lineSource->SetPoint1(points[source.first]->GetPoint(source.second));
                else
                    lineSource->SetPoint1(actors[source.first]->GetUserTransform()->TransformPoint(points[source.first]->GetPoint(source.second)));
                
                if(actors[target.first]->GetUserTransform() == NULL)
                    lineSource->SetPoint2(points[target.first]->GetPoint(target.second));
                else
                    lineSource->SetPoint2(actors[target.first]->GetUserTransform()->TransformPoint(points[target.first]->GetPoint(target.second)));
                
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

int ShapeAnalyzer::getActorId(vtkActor *actor) {
    for(int i = 0; i < MAX_NUM_ACTORS; i++) {
        if(actors[i] == actor) {
            return i;
        }
    }
    return -1;
}

void ShapeAnalyzer::resetCamera() {
    renderer->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}

void ShapeAnalyzer::clear() {
    for(int i = 0; i < numberOfActors; i++) {

        renderer->RemoveActor(actors[i]);
        this->boxWidgets[i]->SetInteractor(NULL);
        this->boxWidgets[i]->SetProp3D(NULL);
        this->data[i] = NULL;
        this->actors[i] = NULL;
        this->boxWidgets[i] = NULL;
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

void ShapeAnalyzer::openShape() {
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.*)"));
    
    if(fileName.count() == 0)
        return;
    
    if(numberOfActors + 1 > MAX_NUM_ACTORS)
        return;
    
    
    vtkSmartPointer<vtkOFFReader> reader = vtkSmartPointer<vtkOFFReader>::New();
    reader->SetFileName(fileName.toUtf8().constData());
    data[numberOfActors] = reader->GetOutput();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
    

    actors[numberOfActors] = vtkSmartPointer<vtkActor>::New();
    actors[numberOfActors]->SetMapper(mapper);

    renderer->AddActor(actors[numberOfActors]);
    
    if(numberOfActors == 0)
        renderer->ResetCamera();
    
    points[numberOfActors] = vtkSmartPointer<vtkPoints>::New();
    
    boxWidgets[numberOfActors] = vtkSmartPointer<vtkBoxWidget>::New();
    boxWidgets[numberOfActors]->SetProp3D(actors[numberOfActors]);
    boxWidgets[numberOfActors]->SetInteractor(qvtkWidget->GetRenderWindow()->GetInteractor());
    boxWidgets[numberOfActors]->SetPlaceFactor(1.25);
    
    boxWidgets[numberOfActors]->PlaceWidget();
    vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
    callback->sa = this;
    
    boxWidgets[numberOfActors]->AddObserver(vtkCommand::InteractionEvent, callback);
    
    qvtkWidget->GetRenderWindow()->Render();
    
    if(this->radioButtonTransformActors->isChecked()) {
        boxWidgets[numberOfActors]->On();
    } else {
        boxWidgets[numberOfActors]->Off();
    }
    
    numberOfActors++;
}

bool ShapeAnalyzer::eventFilter(QObject *object, QEvent *event) {
    if (object == qvtkWidget && event->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}




