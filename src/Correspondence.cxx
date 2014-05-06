#include "Correspondence.h"

// Constructor
Correspondence::Correspondence(
                               Shape* shape1,
                               Shape* shape2,
                               vtkSmartPointer<vtkTriangle> face1,
                               vtkSmartPointer<vtkTriangle> face2,
                               vtkSmartPointer<vtkActor> face1Actor,
                               vtkSmartPointer<vtkActor> face2Actor
) {

    face1_ = face1;
    face2_ = face2;
    
    face1Actor_ = face1Actor;
    face2Actor_ = face2Actor;
    
    //initialize point1 to center of face1
    //method TriangleCenter is static therefore call it as vtkTriangle::TriangleCenter
    //For some strange reason stated in the vtk doc we have to use face1_->GetPoints()->GetPoint(0, p1) with locally initialized p1 instead of p1 = face1_->GetPoints()->GetPoint(0)
    double p1[3];
    double p2[3];
    double p3[3];
    face1_->GetPoints()->GetPoint(0, p1);
    face1_->GetPoints()->GetPoint(1, p2);
    face1_->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, point1_);
    
    //initialize point2 to center of face2
    face2_->GetPoints()->GetPoint(0, p1);
    face2_->GetPoints()->GetPoint(1, p2);
    face2_->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, point2_);

    
    shape1_ = shape1;
    shape2_ = shape2;
    
    
    //Visualize in vtk:
    
    //initialize poly data and required data structures
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    polyData_ = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkCellArray> lineCells = vtkSmartPointer<vtkCellArray>::New();
    
    // create poly data by setting points and inserting cells and putting them both into polydata.
    points->InsertNextPoint(point1_);
    points->InsertNextPoint(point2_);
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);
    lineCells->InsertNextCell(line);
    polyData_->SetPoints(points);
    polyData_->SetLines(lineCells);
    
    //create mapper and actor. mapper maps the poly data to the line actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData_);
    
    vtkLinearTransform* t1 = shape1->getActor()->GetUserTransform();
    if(t1 != nullptr) {
        transformPoint1(t1);
    }
    
    vtkLinearTransform* t2 = shape2->getActor()->GetUserTransform();
    if(t2 != nullptr) {
        transformPoint2(t2);
    }
    
    actor_ = vtkSmartPointer<vtkActor>::New();
    actor_->SetMapper(mapper);
    actor_->GetProperty()->SetLineWidth(1);
    actor_->GetProperty()->SetColor(0, 1, 0);
}

void Correspondence::transformPoint1(vtkLinearTransform* t) {
    polyData_->GetPoints()->SetPoint(0, t->TransformPoint(point1_));
    polyData_->Modified();
    face1Actor_->SetUserTransform(t);
}

void Correspondence::transformPoint2(vtkLinearTransform* t) {
    polyData_->GetPoints()->SetPoint(1, t->TransformPoint(point2_));
    polyData_->Modified();
    face2Actor_->SetUserTransform(t);
}
