//
//  FaceCorrespondencePicker.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "FaceCorrespondencePicker.h"


void FaceCorrespondencePicker::getCurrentSelectionPoint(Shape* shape, vtkIdType faceId, double point[3]) {
    double p1[3];
    double p2[3];
    double p3[3];
    vtkSmartPointer<vtkTriangle> face = vtkTriangle::SafeDownCast(shape->getPolyData()->GetCell(faceId));
    face->GetPoints()->GetPoint(0, p1);
    face->GetPoints()->GetPoint(1, p2);
    face->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, point);
}

void FaceCorrespondencePicker::visualizeCurrentSelection(Shape* shape, vtkIdType faceId) {
    vtkSmartPointer<vtkTriangle> face = vtkTriangle::SafeDownCast(shape->getPolyData()->GetCell(faceId));
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    double p[3];
    face->GetPoints()->GetPoint(0, p);
    points->InsertNextPoint(p);
    face->GetPoints()->GetPoint(1, p);
    points->InsertNextPoint(p);
    face->GetPoints()->GetPoint(2, p);
    points->InsertNextPoint(p);
    
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0, 0);
    triangle->GetPointIds()->SetId(1, 1);
    triangle->GetPointIds()->SetId(2, 2);
    
    polys->InsertNextCell(triangle);
    
    
    polyData->SetPoints(points);
    polyData->SetPolys(polys);

    currentSelectionMapper_->SetInputData(polyData);
    currentSelectionActor_->SetMapper(currentSelectionMapper_);
    currentSelectionActor_->GetProperty()->EdgeVisibilityOn();
    currentSelectionActor_->GetProperty()->SetEdgeColor(0, 1, 1);
    currentSelectionActor_->GetProperty()->SetLineWidth(3);
    currentSelectionActor_->SetUserTransform(shape->getActor()->GetUserTransform());
    
    renderer_->AddActor(currentSelectionActor_);
    renderer_->Render();
}

Correspondence* FaceCorrespondencePicker::createCorrespondence() {
    return new FaceCorrespondence(renderer_, new FaceCorrespondenceData());
}

