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

//serves as a marker class to ensure that variable "HashMap<...> faceCorrespondences" only contains face correspondences
class FaceCorrespondenceData : public CorrespondenceData {
public:
    FaceCorrespondenceData(vtkIdType id) : CorrespondenceData(id) {
    }
    
    virtual string getType() {
        return "FaceCorrespondenceData";
    }
};

#endif /* defined(FaceCorrespondenceData_H) */
