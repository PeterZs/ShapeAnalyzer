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
    void addShape(const vtkIdType newShape, const vtkIdType newCorrespondence) {
        shapeIds_.push_back(newShape);
        correspondingIds_.push_back(newCorrespondence);
    }
    
    void clear() {
        shapeIds_.clear();
        correspondingIds_.clear();
    }
    
    // getters
    vector<vtkIdType>& getShapeIds() {
        return shapeIds_;
    }
    
    vector<vtkIdType>& getCorrespondingIds() {
        return correspondingIds_;
    }
    
    int size() { return shapeIds_.size(); }
    
    
    vtkIdType getId() {
        return id_;
    }
    
    virtual string getType() = 0;
    virtual ~CorrespondenceData() {}
protected:
    CorrespondenceData(vtkIdType id) : id_(id) {}
    
    vector<vtkIdType> shapeIds_;
    vector<vtkIdType> correspondingIds_; // either face or point ids depending on the implementing class
    vtkIdType id_;
};

#endif /* defined(CorrespondenceData_H) */
