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
    // copy constructor
    CorrespondenceData(CorrespondenceData &data) {
        shapeIds_ = getShapes();
        correspondingIds_ = getData();
    }
    
    void addData(const vtkIdType newShape, const vtkIdType newCorrespondence) {
        shapeIds_.push_back(newShape);
        correspondingIds_.push_back(newCorrespondence);
    }
    
    std::vector<vtkIdType> getShapes() {
        return shapeIds_;
    }
    
    std::vector<vtkIdType> getData() {
        return correspondingIds_;
    }
    
    void clear() {
        shapeIds_.clear();
        correspondingIds_.clear();
    }
    
protected:
    CorrespondenceData() {}
    
    std::vector<vtkIdType>& getShapeIds() {
        return shapeIds_;
    }
    
    std::vector<vtkIdType> shapeIds_;
    std::vector<vtkIdType> correspondingIds_; // either face or point ids depending on the implementing class
};

#endif /* defined(CorrespondenceData_H) */
