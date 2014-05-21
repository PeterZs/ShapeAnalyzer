//
//  FaceCorrespondencePicker.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "FaceCorrespondencePicker.h"

vtkSmartPointer<vtkPolyData> FaceCorrespondencePicker::getSelectionPolyData(vtkSmartPointer<vtkShape> shape, vtkIdType faceId) {
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
    
    return polyData;
}

void FaceCorrespondencePicker::getSelectionPoint(vtkSmartPointer<vtkShape> shape, vtkIdType faceId, double point[3]) {
    double p1[3];
    double p2[3];
    double p3[3];
    vtkSmartPointer<vtkTriangle> face = vtkTriangle::SafeDownCast(shape->getPolyData()->GetCell(faceId));
    face->GetPoints()->GetPoint(0, p1);
    face->GetPoints()->GetPoint(1, p2);
    face->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, point);
}

void FaceCorrespondencePicker::createActor(vtkActor *actor, vtkPolyDataMapper *mapper, vtkPolyData *polyData, vtkLinearTransform *t) {
    mapper->SetInputData(polyData);
    actor->SetMapper(mapper);
    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetEdgeColor(0, 1, 1);
    actor->GetProperty()->SetLineWidth(3);
    actor->SetUserTransform(t);
}

Correspondence* FaceCorrespondencePicker::createCorrespondence(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkShape> shape1, vtkSmartPointer<vtkShape> shape2, vtkIdType selectionId, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2) {
    
    return new FaceCorrespondence(renderer, shape1, shape2, id1_, selectionId, actor1, actor2);
}

