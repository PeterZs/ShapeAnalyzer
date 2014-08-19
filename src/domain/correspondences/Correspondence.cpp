#include "Correspondence.h"


///////////////////////////////////////////////////////////////////////////////
Correspondence::Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data) : renderer_(renderer), data_(data) {
    initialize();
}


///////////////////////////////////////////////////////////////////////////////
// to call when the CorrespondenceData is not empty, call produceActor afterwards
Correspondence::Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data, HashMap<vtkActor*, Shape*>* shapes) : renderer_(renderer), data_(data) {
    initialize();
    
    // fill shape_ vector
    for (int i = 0; i < data_->size(); i++) {
        // find right shape
        for (auto it = shapes->begin(); it != shapes->end(); it++) {
            if(data_->getShapes()[i] == it->second->getId()) {
                shapes_.push_back(it->second);
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void Correspondence::initialize() {
    lineReferencePoints_ = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> linePoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lineCells = vtkSmartPointer<vtkCellArray>::New();
    
    linesPolyData_ = vtkSmartPointer<vtkPolyData>::New();
    //initialize poly data
    linesPolyData_->SetPoints(linePoints);
    linesPolyData_->SetLines(lineCells);
    
    
    //create mapper and actor. mapper maps the poly data to the line actor
    linesMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    linesMapper_->SetInputData(linesPolyData_);
    linesActor_ = vtkSmartPointer<vtkActor>::New();
    linesActor_->SetMapper(linesMapper_);
    linesActor_->GetProperty()->SetLineWidth(1);
    linesActor_->GetProperty()->SetColor(0, 1, 0);
    renderer_->AddActor(linesActor_);
}


///////////////////////////////////////////////////////////////////////////////
int Correspondence::addShape(Shape* shape, vtkIdType id) {
    if(shapes_.size() > 0 && shapes_[shapes_.size()-1] == shape) {
        // in case shape is equal to last added shape replace point
        //get coordinates of line target
        double point[3];
        getCorrespondencePoint(point, shape, id);
        
        // shape is equal to the last inserted shape. Replace id and shape
        lineReferencePoints_->SetPoint(lineReferencePoints_->GetNumberOfPoints()-1, point);
        linesPolyData_->GetPoints()->SetPoint(linesPolyData_->GetPoints()->GetNumberOfPoints()-1, point);
        linesPolyData_->Modified();
        
        initializeActor(actors_[actors_.size()-1], shape, id);
        transform(shape);
        return 0;
    }
    
    if(find(shapes_.begin(), shapes_.end(), shape) != shapes_.end()) {
        //shape is already part of correspondence and not equal to the last added
        return -1;
    }
    
    //shape has not yet been added
    shapes_.push_back(shape);
    double point[3];
    getCorrespondencePoint(point, shape, id);
    lineReferencePoints_->InsertNextPoint(point);
    linesPolyData_->GetPoints()->InsertNextPoint(point);
    linesPolyData_->Modified();
    actors_.push_back(vtkSmartPointer<vtkActor>::New());
    initializeActor(actors_[actors_.size()-1], shape, id);
    transform(shape);
    
    data_->addData(shape->getId(), id);
    
    if(shapes_.size() > 1) {
        //Visualize in vtk
        // create new line cell out of the two points inserted last
        vtkSmartPointer<vtkIdList> line = vtkSmartPointer<vtkIdList>::New();
        line->InsertId(0, linesPolyData_->GetPoints()->GetNumberOfPoints()-2);
        line->InsertId(1, linesPolyData_->GetPoints()->GetNumberOfPoints()-1);
        linesPolyData_->InsertNextCell(VTK_LINE, line);
        
        linesPolyData_->Modified();
    }
    
    return 1;
}


///////////////////////////////////////////////////////////////////////////////
// removes correspondence actor from renderer
void Correspondence::remove() {
    for(int i = 0; i < actors_.size(); i++) {
        renderer_->RemoveActor(actors_[i]);
    }
    
    renderer_->RemoveActor(linesActor_);
}

///////////////////////////////////////////////////////////////////////////////
// add correspondence actor to renderer
void Correspondence::add() {
    for(int i = 0; i < actors_.size(); i++) {
        renderer_->AddActor(actors_[i]);
    }
    
    renderer_->AddActor(linesActor_);
}


///////////////////////////////////////////////////////////////////////////////
void Correspondence::produceActorFromData() {
    // fill poly data with all points
    for (int i = 0; i < data_->size(); i++) {
        // insert point
        double point[3];
        getCorrespondencePoint(point, shapes_[i], data_->getCorrespondingIds()[i]);
        linesPolyData_->GetPoints()->InsertNextPoint(point);
        lineReferencePoints_->InsertNextPoint(point);
    }
    
    // produce lines
    for (int i = 1; i < data_->size(); i++) {
        vtkSmartPointer<vtkIdList> line = vtkSmartPointer<vtkIdList>::New();
        line->InsertId(0, i-1);
        line->InsertId(1, i);
        linesPolyData_->InsertNextCell(VTK_LINE, line);
        
        linesPolyData_->Modified();
    }
    
    // transform
    for (int i = 0; i < shapes_.size(); i++) {
        transform(shapes_[i]);
    }
}


///////////////////////////////////////////////////////////////////////////////
void Correspondence::setSelected(bool selected) {
    if(selected) {
        linesActor_->GetProperty()->SetColor(1, 0, 0);
        for(int i = 0; i < actors_.size(); i++) {
            actors_[i]->GetProperty()->SetColor(1, 0, 0);
            actors_[i]->GetProperty()->SetEdgeColor(1, 0, 0);
            renderer_->AddActor(actors_[i]);
        }
    } else {
        linesActor_->GetProperty()->SetColor(0, 1, 0);
        for(int i = 0; i < actors_.size(); i++) {
            renderer_->RemoveActor(actors_[i]);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void Correspondence::transform(Shape* shape) {
    for(int i = 0; i < shapes_.size(); i++) {
        if(shapes_[i] == shape) {
            vtkLinearTransform* t = shape->getActor()->GetUserTransform();
            if(t != nullptr) {
                double point[3];
                lineReferencePoints_->GetPoint(i, point);
                linesPolyData_->GetPoints()->SetPoint(i, t->TransformPoint(point));
                linesPolyData_->Modified();
                actors_[i]->SetUserTransform(t);
            }
        }
    }
}


