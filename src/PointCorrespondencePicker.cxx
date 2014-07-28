//
//  PointCorrespondencePicker.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "PointCorrespondencePicker.h"

///////////////////////////////////////////////////////////////////////////////
void PointCorrespondencePicker::getCurrentSelectionPoint(Shape* shape, vtkIdType pointId, double point[3]) {
    shape->getPolyData()->GetPoint(pointId, point);
}


///////////////////////////////////////////////////////////////////////////////
void PointCorrespondencePicker::visualizeCurrentSelection(Shape* shape, vtkIdType pointId) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    double p[3];
    shape->getPolyData()->GetPoint(pointId, p);
    points->InsertNextPoint(p);
    
    polyData->SetPoints(points);
    
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polyData);
    glyphFilter->Update();
    
    currentSelectionMapper_->SetInputData(glyphFilter->GetOutput());
    currentSelectionActor_->SetMapper(currentSelectionMapper_);
    currentSelectionActor_->GetProperty()->SetColor(0, 1, 1);
    currentSelectionActor_->GetProperty()->SetPointSize(5);
    currentSelectionActor_->GetProperty()->SetRepresentationToPoints();
    currentSelectionActor_->SetUserTransform(shape->getActor()->GetUserTransform());
    renderer_->AddActor(currentSelectionActor_);
}


///////////////////////////////////////////////////////////////////////////////
Correspondence* PointCorrespondencePicker::createCorrespondence() {
    return new PointCorrespondence(renderer_, (PointCorrespondenceData*) data_);
}


