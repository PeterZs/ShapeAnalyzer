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
#include "FaceCorrespondence.h"

class FaceCorrespondencePicker : public CorrespondencePicker {
public:
    FaceCorrespondencePicker(vtkRenderer* renderer) : CorrespondencePicker(renderer) {

    }
    
    virtual vtkSmartPointer<vtkPolyData> getSelectionPolyData(vtkSmartPointer<vtkShape> shape, vtkIdType faceId);
    
    virtual void getSelectionPoint(vtkSmartPointer<vtkShape> shape, vtkIdType faceId, double point[3]);
     
    virtual void createActor(vtkActor* actor, vtkPolyDataMapper* mapper, vtkPolyData* polyData, vtkLinearTransform* t);
    
    virtual Correspondence* createCorrespondence(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkShape> shape1, vtkSmartPointer<vtkShape> shape2, vtkIdType selectionId, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2);
    
    virtual ~FaceCorrespondencePicker() {}
};


#endif /* defined(FaceCorrespondencePicker_H) */
