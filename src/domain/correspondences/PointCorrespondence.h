//
//  PointCorrespondence.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef PointCorrespondence_H
#define PointCorrespondence_H

#include <vtkVertexGlyphFilter.h>

#include "Correspondence.h"
#include "PointCorrespondenceData.h"

#include "../HashMap.h"
#include "../Shape.h"

///
/// \brief Class that represents a concrete PointCorrespondence.
///
/// \details Implements the abstract functions initializeActor() and getCorrespondencePoint() of the superclass to provide a specific visualization behavior for point correspondences.
///
/// \author Emanuel Laude and Zorah Lähner
///
class PointCorrespondence : public Correspondence {
public:
    PointCorrespondence(vtkSmartPointer<vtkRenderer> renderer, PointCorrespondenceData* data);
    PointCorrespondence(vtkSmartPointer<vtkRenderer> renderer, PointCorrespondenceData* data, HashMap<vtkActor*, Shape*>& shapes);
    
    PointCorrespondenceData* getData() { return (PointCorrespondenceData*) data_; }
    
protected:
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType pointId);
    
    virtual void getCorrespondencePoint(double point[3], Shape* shape, vtkIdType);

};
#endif /* defined(PointCorrespondence_H) */
