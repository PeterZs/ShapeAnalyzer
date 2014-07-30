//
//  PointCorrespondeceData.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef PointCorrespondeceData_H
#define PointCorrespondeceData_H

#include "CorrespondenceData.h"

class PointCorrespondenceData : public CorrespondenceData {
public:
    PointCorrespondenceData() {
    }
    
    vector<vtkIdType>& getPointIds() {
        return correspondingIds_;
    }
};

#endif /* defined(PointCorrespondeceData_H) */
