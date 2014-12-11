#ifndef __ShapeAnalyzer__PointCorrespondence__
#define __ShapeAnalyzer__PointCorrespondence__

#include "Correspondence.h"

///
/// \brief Stores multi correspondences between points of an arbitrary number of shapes.
/// \details The main implementation can be seen in CorrespondenceData, this class
/// purely insures that point and face correspondences can not be mixed up in some
/// data structures (especially pointCorrespondenceData_ in ShapeAnalyzer).
///
class PointCorrespondence : public Correspondence {
public:
    /// \brief Basic Constructor.
    /// @param id The final id of this correspondence data. It should be unique
    /// among all correspondence data, but this is not enforced.
    PointCorrespondence(vtkIdType id) : Correspondence(id) {
    }
    
    virtual string toString() {
        string str;
        
        str.append("Type: ");
        str.append("PointCorrespondence");
        str.append("\n");
        
        str.append("ID: ");
        str.append(to_string(id_));
        str.append("\n");
        
        
        for(int i = 0; i < shapes_.size(); i++) {
            str.append("S");
            str.append(to_string(shapes_[i]->getId()));
            str.append(":");
            str.append(to_string(correspondingIds_[i]));
            if(i < shapes_.size()-1) {
                str.append("->");
            }
        }
        
        return str;
    }
};

#endif /* defined(__ShapeAnalyzer__PointCorrespondence__) */
