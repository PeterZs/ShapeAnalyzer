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

#include <vector>

class CorrespondenceData {
public:

    
protected:
    CorrespondenceData() {}
    
    vector<vtkIdType>& getShapeIds() {
        return shapeIds_;
    }
    
private:
    vector<vtkIdType> shapeIds_;
};

#endif /* defined(CorrespondenceData_H) */
