//
//  CorrespondenceData.h
//  ShapeAnalyzer
//
//  Abstract class to store correspondence information. 
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
    // manage data
    void addData(const vtkIdType newShape, const vtkIdType newCorrespondence) {
        shapeIds_.push_back(newShape);
        correspondingIds_.push_back(newCorrespondence);
    }
    
    void clear() {
        shapeIds_.clear();
        correspondingIds_.clear();
    }
    
    // getters
    vector<vtkIdType>& getShapes() {
        return shapeIds_;
    }
    
    vector<vtkIdType>& getCorrespondingIds() {
        return correspondingIds_;
    }
    
protected:
    CorrespondenceData() {}
    
    vector<vtkIdType> shapeIds_;
    vector<vtkIdType> correspondingIds_; // either face or point ids depending on the implementing class
};

#endif /* defined(CorrespondenceData_H) */
