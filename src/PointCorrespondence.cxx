//
//  PointCorrespondence.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "PointCorrespondence.h"

PointCorrespondence::PointCorrespondence(vtkSmartPointer<vtkRenderer> renderer, PointCorrespondenceData* data) : Correspondence(renderer, data) {
}

// create actor that visualizes selected point on shape
void PointCorrespondence::initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType pointId) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    double p[3];
    shape->getPolyData()->GetPoint(pointId, p);
    points->InsertNextPoint(p);
    
    polyData->SetPoints(points);
    
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polyData);
    glyphFilter->Update();
    
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputData(glyphFilter->GetOutput());
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(5);
    actor->GetProperty()->SetRepresentationToPoints();
    actor->GetProperty()->SetColor(0, 1, 0);
    actor->SetUserTransform(shape->getActor()->GetUserTransform());
}

// returns point that serves as source or target of correspondence line
void PointCorrespondence::getCorrespondencePoint(double point[3], Shape* shape, vtkIdType pointId) {
    shape->getPolyData()->GetPoints()->GetPoint(pointId, point);
}