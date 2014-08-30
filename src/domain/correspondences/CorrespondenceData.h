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
#include <iostream>
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
    
    string toString() {
        string str;
        
        str.append("Type: ");
        str.append(getType());
        str.append("\n");
        
        str.append("ID: ");
        str.append(to_string(id_));
        str.append("\n");
        

        for(int i = 0; i < shapeIds_.size(); i++) {
            str.append("S");
            str.append(to_string(shapeIds_[i]));
            str.append(":");
            str.append(to_string(correspondingIds_[i]));
            if(i < shapeIds_.size()-1) {
                str.append("->");
            }
        }
        
        return str;
    }
    
    virtual string getType() = 0;
    virtual ~CorrespondenceData() {
    }
protected:
    CorrespondenceData(vtkIdType id) : id_(id) {}
    
    vector<vtkIdType> shapeIds_;
    vector<vtkIdType> correspondingIds_; // either face or point ids depending on the implementing class
    vtkIdType id_;
};

#endif /* defined(CorrespondenceData_H) */
