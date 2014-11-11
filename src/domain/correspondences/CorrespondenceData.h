#ifndef CorrespondenceData_H
#define CorrespondenceData_H

#include <vtkType.h>

#include <vector>
#include <iostream>
using namespace std;

///
/// \brief Abstract class to store correspondence information.
///
/// \details CorrespondenceData matches data ids of an arbitrary number of shapes
/// to each other. Each CorrespondenceData has a unique id and stores a vector
/// of shape ids and a vector of data ids with the same size. The ids on the same
/// position of each vector belong to each other.
/// It is possible to add more data after initialization, but not to delete it.
///
/// \author Emanuel Laude and Zorah Lähner
///
class CorrespondenceData {
    
public:    
    /// \brief Adds a new shape and data id to the object.
    void addShape(const vtkIdType newShape, const vtkIdType newCorrespondence) {
        shapeIds_.push_back(newShape);
        correspondingIds_.push_back(newCorrespondence);
    }
    
    /// \brief Clears the Correspondence Data without deleting the object.
    void clear() {
        shapeIds_.clear();
        correspondingIds_.clear();
    }
    
    /// \brief Returns a pointer to the vector of shape ids that are in the
    /// object.
    vector<vtkIdType>& getShapeIds() {
        return shapeIds_;
    }
    
    /// \brief Returns a pointer to the vector of data ids that are in the
    /// object.
    vector<vtkIdType>& getCorrespondingIds() {
        return correspondingIds_;
    }
    
    /// \brief Returns the number of shapes that have corresponding data.
    int size() { return shapeIds_.size(); }
    
    
    /// Returns the id of this object.
    vtkIdType getId() {
        return id_;
    }
    
    /// Returns a string with all information of this CorrespondenceData.
    /// @return The resulting string has the form: \n
    /// Type: _T_ \n
    /// ID: _ID_ \n
    /// S _SID_ : _DID_ -> ... -> S _SID_ : _DID_ \n
    /// where _T_ is the type of the object (has to be implemented in the subclass)
    /// _ID_ is the id of the object, _SID_ is the id of a shape in this object,
    /// _DID_ is the id of the data in the shape with _SID_.
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
    
    /// Virtual method, returns a string that describes the type of correspondence, i.e. point correspondence.
    virtual string getType() = 0;
    /// Empty Destructor.
    virtual ~CorrespondenceData() {
    }
protected:
    /// Protected Constructor.
    /// @param id Will be the Id of this object. It should be unique, but no further checking is
    /// made.
    CorrespondenceData(vtkIdType id) : id_(id) {}
    
    /// \brief Vector of the ids of the corresponding shapes.
    vector<vtkIdType> shapeIds_;
    /// \brief Vector of the ids of the corresponding data.
    /// \details The id at position i refers to data in the shape with
    /// the shape id found at shapeIds_[i].
    vector<vtkIdType> correspondingIds_; // either face or point ids depending on the implementing class
    /// \brief correspondence id.
    vtkIdType id_;
};

#endif /* defined(CorrespondenceData_H) */
