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
#include "FaceCorrespondenceData.h"

#include "../HashMap.h"
#include "../Shape.h"

class FaceCorrespondence : public Correspondence {
public:
    FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, string label, FaceCorrespondenceData* data);
    FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, string label, FaceCorrespondenceData* data, HashMap<vtkActor*, Shape*>& shapes);
    
    FaceCorrespondenceData* getData() { return (FaceCorrespondenceData*) data_; }
    
protected:
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType faceId);
    
    virtual void getCorrespondencePoint(double point[3], Shape* shape, vtkIdType);
};

#endif /* defined(FaceCorrespondence_H) */
