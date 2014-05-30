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
    FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, FaceCorrespondenceData* data);
    
private:
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType faceId);
    
    virtual void getCorrespondencePoint(double point[3], Shape* shape, vtkIdType);
};

#endif /* defined(FaceCorrespondence_H) */
