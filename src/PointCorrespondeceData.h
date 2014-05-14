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
    PointCorrespondenceData(vtkIdType shape1Id, vtkIdType shape2Id, vtkIdType point1Id, vtkIdType point2Id) : CorrespondenceData(shape1Id, shape2Id) {
        point1Id_ = point1Id;
        point2Id_ = point2Id;
    }
    
    vtkIdType getPoint1Id() {
        return point1Id_;
    }
    
    vtkIdType getPoint2Id() {
        return point2Id_;
    }
    
private:
    vtkIdType point1Id_;
    vtkIdType point2Id_;
};

#endif /* defined(PointCorrespondeceData_H) */
