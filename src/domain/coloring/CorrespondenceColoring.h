//
//  CorrespondenceColoring.h
//  ShapeAnalyzer
//
// Colors the given shapes to visualize the given correspondences.
// The reference shape will be colored by coordinates and
// all points with a corresponding point on another shape will be colored
// in the same way as the correspondence point
//
//  Created by Zorah on 08.07.14.
//
//

#ifndef __ShapeAnalyzer__CorrespondenceColoring__
#define __ShapeAnalyzer__CorrespondenceColoring__

#include "CoordinateColoring.h"
#include "FaceColoring.h"
#include "FaceCoordinateColoring.h"
#include "PointColoring.h"

#include <vtkSmartPointer.h>
#include <vtkType.h>
#include <vtkUnsignedCharArray.h>

#include "../Shape.h"
#include "../Set.h"

#include "../correspondences/FaceCorrespondence.h"
#include "../correspondences/PointCorrespondenceData.h"

#include "../attributes/ScalarPointAttribute.h"
#include "../attributes/ScalarFaceAttribute.h"

using namespace std;

class CorrespondenceColoring {
public:
    CorrespondenceColoring(Set<vtkActor*, Shape*>*              set,
                           Set<PointCorrespondenceData*, bool>* points,
                           Set<FaceCorrespondenceData*, bool>*  faces,
                           Shape* reference = 0);
    
    void showPointCorrespondences();
    void showFaceCorrespondences();
    
protected:
    Set<vtkActor*, Shape*>*                 shapes_;
    Set<PointCorrespondenceData*, bool>*    points_;
    Set<FaceCorrespondenceData*, bool>*     faces_;
    
    Shape* reference_;
    
    unordered_map<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >  pointAttributes_;
    unordered_map<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >  faceAttributes_;
    
};

#endif /* defined(__ShapeAnalyzer__CorrespondenceColoring__) */
