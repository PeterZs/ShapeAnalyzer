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
    FaceCorrespondencePicker(vtkRenderer* renderer) : CorrespondencePicker(renderer) {
        data_ = new FaceCorrespondenceData();
    }
private:
    virtual void getCurrentSelectionPoint(Shape* shape, vtkIdType, double point[3]);
    
    virtual void visualizeCurrentSelection(Shape* shape, vtkIdType pointId);
    
    virtual Correspondence* createCorrespondence();
    
    virtual ~FaceCorrespondencePicker() {}
};


#endif /* defined(FaceCorrespondencePicker_H) */
