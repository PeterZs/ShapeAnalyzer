//
//  FaceCorrespondenceData.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef FaceCorrespondenceData_H
#define FaceCorrespondenceData_H

#include "CorrespondenceData.h"

class FaceCorrespondenceData : public CorrespondenceData {
public:
    FaceCorrespondenceData(vtkIdType shape1Id, vtkIdType shape2Id, vtkIdType face1Id, vtkIdType face2Id) : CorrespondenceData(shape1Id, shape2Id) {
        face1Id_ = face1Id;
        face2Id_ = face2Id;
    }
    
    vtkIdType getFace1Id() {
        return face1Id_;
    }
    
    vtkIdType getFace2Id() {
        return face2Id_;
    }
    
private:
    vtkIdType face1Id_;
    vtkIdType face2Id_;
};

#endif /* defined(FaceCorrespondenceData_H) */
