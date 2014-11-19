#ifndef FaceCorrespondence_H
#define FaceCorrespondence_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkTriangle.h>

#include "Correspondence.h"
#include "FaceCorrespondenceData.h"

#include "../../util/HashMap.h"
#include "../Shape.h"

///
/// \brief Class that represents a concrete FaceCorrespondence.
///
/// \details Implements the abstract functions initializeActor() and getCorrespondencePoint() of the superclass to provide a specific visualization behavior for face correspondences.
///
/// \author Emanuel Laude and Zorah Lähner
///
class FaceCorrespondence : public Correspondence {
public:
    FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, FaceCorrespondenceData* data);
    FaceCorrespondence(vtkSmartPointer<vtkRenderer> renderer, FaceCorrespondenceData* data, HashMap<vtkActor*, Shape*>& shapes);
    
    FaceCorrespondenceData* getData() { return (FaceCorrespondenceData*) data_; }
    
protected:
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType faceId);
    
    virtual void getCorrespondencePoint(double point[3], Shape* shape, vtkIdType);
};

#endif /* defined(FaceCorrespondence_H) */
