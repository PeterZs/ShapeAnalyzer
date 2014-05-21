//
//  PointCorrespondencePicker.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "PointCorrespondencePicker.h"

vtkSmartPointer<vtkPolyData> PointCorrespondencePicker::getSelectionPolyData(vtkSmartPointer<vtkShape> shape, vtkIdType pointId) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    double p[3];
    shape->getPolyData()->GetPoint(pointId, p);
    points->InsertNextPoint(p);

    polyData->SetPoints(points);
    
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polyData);
    glyphFilter->Update();
    
    return glyphFilter->GetOutput();
}

void PointCorrespondencePicker::getSelectionPoint(vtkSmartPointer<vtkShape> shape, vtkIdType pointId, double point[3]) {
    shape->getPolyData()->GetPoint(pointId, point);
}

Correspondence* PointCorrespondencePicker::createCorrespondence(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkShape> shape1, vtkSmartPointer<vtkShape> shape2, vtkIdType selectionId, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2) {

    return new PointCorrespondence(renderer, shape1, shape2, id1_, selectionId, actor1, actor2);
}

void PointCorrespondencePicker::createActor(vtkActor *actor, vtkPolyDataMapper *mapper, vtkPolyData *polyData, vtkLinearTransform *t) {
    mapper->SetInputData(polyData);
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(5);
    actor->GetProperty()->SetRepresentationToPoints();
    actor->GetProperty()->SetColor(0, 1, 1);
    actor->GetProperty()->SetLineWidth(1);
    actor->SetUserTransform(t);
}



