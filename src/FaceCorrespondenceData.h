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
    FaceCorrespondenceData() {
    }
    
    vector<vtkIdType>& getFaceIds() {
        return faceIds_;
    }
    
private:
    vector<vtkIdType> faceIds_;
};

#endif /* defined(FaceCorrespondenceData_H) */
