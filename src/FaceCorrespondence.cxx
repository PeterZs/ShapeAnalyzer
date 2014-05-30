//
//  FaceCorrespondence.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "FaceCorrespondence.h"

FaceCorrespondence::FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, FaceCorrespondenceData* data) : Correspondence(renderer, data) {
}

// create actor that visualizes selected point on shape
void FaceCorrespondence::initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType faceId) {
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
    
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    
    mapper->SetInputData(polyData);
    actor->SetMapper(mapper);
    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetEdgeColor(0, 1, 0);
    actor->GetProperty()->SetLineWidth(3);
    actor->SetUserTransform(shape->getActor()->GetUserTransform());
}

// returns point that serves as source or target of correspondence line
void FaceCorrespondence::getCorrespondencePoint(double point[3], Shape* shape, vtkIdType faceId) {
    double p1[3];
    double p2[3];
    double p3[3];
    
    vtkSmartPointer<vtkTriangle> face = vtkTriangle::SafeDownCast(shape->getPolyData()->GetCell(faceId));
    face->GetPoints()->GetPoint(0, p1);
    face->GetPoints()->GetPoint(1, p2);
    face->GetPoints()->GetPoint(2, p3);
    
    vtkTriangle::TriangleCenter(p1, p2, p3, point);
}


