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

#include "../domain/correspondences/PointCorrespondence.h"
#include "CorrespondencePicker.h"

class PointCorrespondencePicker : public CorrespondencePicker {
public:
    PointCorrespondencePicker(vtkRenderer* renderer, int& lastInsertCorrespondenceID) : CorrespondencePicker(renderer, lastInsertCorrespondenceID) {
    }
private:
    virtual void getCurrentSelectionPoint(Shape* shape, vtkIdType, double point[3]) throw(std::invalid_argument);
    
    virtual void visualizeCurrentSelection(Shape* shape, vtkIdType pointId) throw(std::invalid_argument);
    
    virtual Correspondence* createCorrespondence();

    virtual ~PointCorrespondencePicker() {}
};

#endif /* defined(PointCorrespondencePicker_H) */
