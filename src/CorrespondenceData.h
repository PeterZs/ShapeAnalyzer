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

using namespace std;

class CorrespondenceData {
public:

    
protected:
    CorrespondenceData() {}
    
    std::vector<vtkIdType>& getShapeIds() {
        return shapeIds_;
    }
    
private:
    std::vector<vtkIdType> shapeIds_;
};

#endif /* defined(CorrespondenceData_H) */
