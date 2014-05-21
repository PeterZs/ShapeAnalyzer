//
//  PointCorrespondencePicker.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef PointCorrespondencePicker_H
#define PointCorrespondencePicker_H

#include <vtkVertexGlyphFilter.h>

#include "PointCorrespondence.h"
#include "CorrespondencePicker.h"

class PointCorrespondencePicker : public CorrespondencePicker {
public:
    PointCorrespondencePicker(vtkRenderer* renderer) : CorrespondencePicker(renderer) {
    
    }
    
    virtual vtkSmartPointer<vtkPolyData> getSelectionPolyData(vtkSmartPointer<vtkShape> shape, vtkIdType pointId);
    
    virtual void getSelectionPoint(vtkSmartPointer<vtkShape> shape, vtkIdType pointId, double point[3]);
    
    virtual void createActor(vtkActor* actor, vtkPolyDataMapper* mapper, vtkPolyData* polyData, vtkLinearTransform* t);
    
    virtual Correspondence* createCorrespondence(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkShape> shape1, vtkSmartPointer<vtkShape> shape2, vtkIdType selectionId, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2);

    virtual ~PointCorrespondencePicker() {}
};

#endif /* defined(PointCorrespondencePicker_H) */
