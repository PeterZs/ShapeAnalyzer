//
//  FaceCorrespondencePicker.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef FaceCorrespondencePicker_H
#define FaceCorrespondencePicker_H

#include "CorrespondencePicker.h"
#include "../domain/correspondences/FaceCorrespondence.h"

class FaceCorrespondencePicker : public CorrespondencePicker {
public:
    FaceCorrespondencePicker(vtkRenderer* renderer, int& lastInsertCorrespondenceID) : CorrespondencePicker(renderer, lastInsertCorrespondenceID) {
    }
private:
    /// \throws invalid_argument Shape null pointer.
    /// \throws invalid_argument Shape faceId is larger than the number of faces in the shape.
    virtual void getCurrentSelectionPoint(Shape* shape, vtkIdType faceId, double point[3]);
    
    /// \throws invalid_argument Shape null pointer.
    /// \throws invalid_argument Shape faceId is larger than the number of faces in the shape.
    virtual void visualizeCurrentSelection(Shape* shape, vtkIdType faceId);
    
    virtual Correspondence* createCorrespondence();
    
    virtual ~FaceCorrespondencePicker() {}
};


#endif /* defined(FaceCorrespondencePicker_H) */
