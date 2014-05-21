//
//  FaceCorrespondence.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "FaceCorrespondence.h"

FaceCorrespondence::FaceCorrespondence(
                                       vtkSmartPointer<vtkRenderer> renderer,
                                       vtkSmartPointer<vtkShape> shape1,
                                       vtkSmartPointer<vtkShape> shape2,
                                       vtkIdType face1Id, vtkIdType face2Id,
                                       vtkSmartPointer<vtkActor> actor1,
                                       vtkSmartPointer<vtkActor> actor2
                                       ) : Correspondence(renderer, shape1, shape2, actor1, actor2) {
    
    data_ = new FaceCorrespondenceData(shape1->getId(), shape2->getId(), face1Id, face2Id);

    double p1[3];
    double p2[3];
    double p3[3];
    
    vtkSmartPointer<vtkTriangle> face1 = vtkTriangle::SafeDownCast(shape1->getPolyData()->GetCell(face1Id));
    face1->GetPoints()->GetPoint(0, p1);
    face1->GetPoints()->GetPoint(1, p2);
    face1->GetPoints()->GetPoint(2, p3);
    
    double point1[3];
    vtkTriangle::TriangleCenter(p1, p2, p3, point1);
    
    //initialize point2 to center of face2
    vtkSmartPointer<vtkTriangle> face2 = vtkTriangle::SafeDownCast(shape2->getPolyData()->GetCell(face2Id));
    face2->GetPoints()->GetPoint(0, p1);
    face2->GetPoints()->GetPoint(1, p2);
    face2->GetPoints()->GetPoint(2, p3);
    
    double point2[3];
    vtkTriangle::TriangleCenter(p1, p2, p3, point2);
    
    Correspondence::visualize(point1, point2);
}

