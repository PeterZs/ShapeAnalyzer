#include "Correspondence.h"


///////////////////////////////////////////////////////////////////////////////
Correspondence::Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data) : renderer_(renderer), data_(data) {
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
int Correspondence::add(Shape* shape, vtkIdType id) {
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
    
    //data_->addData(shape->getId(), id);
    
    if(shapes_.size() > 1) {
        //Visualize in vtk
        //vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
        
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


