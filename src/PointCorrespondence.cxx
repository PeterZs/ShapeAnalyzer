//
//  PointCorrespondence.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#include "PointCorrespondence.h"

PointCorrespondence::PointCorrespondence(vtkSmartPointer<vtkRenderer> renderer, Shape* shape1, Shape* shape2, vtkIdType point1Id, vtkIdType point2Id, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2) : Correspondence(renderer, shape1, shape2, actor1, actor2) {
    
    data_ = new PointCorrespondenceData(shape1->getId(), shape2->getId(), point1Id, point2Id);
    
    double point1[3];
    shape1->getPolyData()->GetPoint(point1Id, point1);

    double point2[3];
    shape2->getPolyData()->GetPoint(point2Id, point2);
    
    Correspondence::visualize(point1, point2);
}
