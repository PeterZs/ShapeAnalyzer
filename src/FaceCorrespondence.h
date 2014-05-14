//
//  FaceCorrespondence.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef FaceCorrespondence_H
#define FaceCorrespondence_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkTriangle.h>

#include "Correspondence.h"
#include "Shape.h"
#include "FaceCorrespondenceData.h"

class FaceCorrespondence : public Correspondence {
public:
    FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, Shape* shape1, Shape* shape2, vtkIdType face1Id, vtkIdType face2Id, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2);
};

#endif /* defined(FaceCorrespondence_H) */
