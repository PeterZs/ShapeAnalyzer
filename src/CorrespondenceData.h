//
//  CorrespondenceData.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef CorrespondenceData_H
#define CorrespondenceData_H

#include <vtkType.h>

class CorrespondenceData {
public:
    vtkIdType getShape1Id() {
        return shape1Id_;
    }
    
    vtkIdType getShape2Id() {
        return shape2Id_;
    }
    
protected:
    CorrespondenceData(vtkIdType shape1Id, vtkIdType shape2Id) {
        shape1Id_ = shape1Id;
        shape2Id_ = shape2Id;
    }
    
private:
    vtkIdType shape1Id_;
    vtkIdType shape2Id_;
};

#endif /* defined(CorrespondenceData_H) */
