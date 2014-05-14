#include "Correspondence.h"

// Constructor
Correspondence::Correspondence(vtkSmartPointer<vtkRenderer> renderer, Shape* shape1, Shape* shape2, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2) {
    shape1_ = shape1;
    shape2_ = shape2;

    actor1_ = actor1;
    actor2_ = actor2;
    renderer_ = renderer;
}

//
void Correspondence::visualize(double point1[3], double point2[3]) {
    //copy points
    point1_[0] = point1[0];
    point1_[1] = point1[1];
    point1_[2] = point1[2];

    point2_[0] = point2[0];
    point2_[1] = point2[1];
    point2_[2] = point2[2];

    
    //Visualize in vtk:
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    
    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkCellArray> lineCells = vtkSmartPointer<vtkCellArray>::New();
    
    // create poly data by setting points and inserting cells and putting them both into polydata.
    points->InsertNextPoint(point1_);
    points->InsertNextPoint(point2_);
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);
    lineCells->InsertNextCell(line);
    
    //initialize poly data
    linePolyData_ = vtkSmartPointer<vtkPolyData>::New();
    linePolyData_->SetPoints(points);
    linePolyData_->SetLines(lineCells);
    
    //create mapper and actor. mapper maps the poly data to the line actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(linePolyData_);
    
    vtkLinearTransform* t1 = shape1_->getActor()->GetUserTransform();
    if(t1 != nullptr) {
        transform1(t1);
    }
    
    vtkLinearTransform* t2 = shape2_->getActor()->GetUserTransform();
    if(t2 != nullptr) {
        transform2(t2);
    }
    
    lineActor_ = vtkSmartPointer<vtkActor>::New();
    lineActor_->SetMapper(mapper);
    lineActor_->GetProperty()->SetLineWidth(1);
    lineActor_->GetProperty()->SetColor(0, 1, 0);
    renderer_->AddActor(lineActor_);
}

void Correspondence::remove() {
    renderer_->RemoveActor(actor1_);
    renderer_->RemoveActor(actor2_);
    
    renderer_->RemoveActor(lineActor_);
}

void Correspondence::setSelected(bool selected) {
    if(selected) {
        lineActor_->GetProperty()->SetColor(1, 0, 0);
        actor1_->GetProperty()->SetColor(1, 0, 0);
        actor1_->GetProperty()->SetEdgeColor(1, 0, 0);
        renderer_->AddActor(actor1_);
        actor2_->GetProperty()->SetColor(1, 0, 0);
        actor2_->GetProperty()->SetEdgeColor(1, 0, 0);
        renderer_->AddActor(actor2_);
    } else {
        lineActor_->GetProperty()->SetColor(0, 1, 0);
        renderer_->RemoveActor(actor1_);
        renderer_->RemoveActor(actor2_);
    }
}

void Correspondence::transform1(vtkLinearTransform* t) {
    linePolyData_->GetPoints()->SetPoint(0, t->TransformPoint(point1_));
    linePolyData_->Modified();
    actor1_->SetUserTransform(t);
}

void Correspondence::transform2(vtkLinearTransform* t) {
    linePolyData_->GetPoints()->SetPoint(1, t->TransformPoint(point2_));
    linePolyData_->Modified();
    actor2_->SetUserTransform(t);
}
