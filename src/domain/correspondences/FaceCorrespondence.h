#ifndef __ShapeAnalyzer__FaceCorrespondence__
#define __ShapeAnalyzer__FaceCorrespondence__

#include "Correspondence.h"

///
/// \brief Stores multi correspondences between faces of an arbitrary number of shapes.
/// \details The main implementation can be seen in CorrespondenceData, this class
/// purely insures that point and face correspondences can not be mixed up in some
/// data structures (especially faceCorrespondencesData_ in ShapeAnalyzer).
///
class FaceCorrespondence : public Correspondence {
public:
    /// \brief Basic Constructor.
    /// @param id The final id of this correspondence data. It should be unique
    /// among all correspondence data, but this is not enforced.
    FaceCorrespondence(vtkIdType id) : Correspondence(id) {
    }
    
    virtual string toString() {
        string str;
        
        str.append("Type: ");
        str.append("FaceCorrespondence");
        str.append("\n");
        
        str.append("ID: ");
        str.append(to_string(id_));
        str.append("\n");
        
        
        for(int i = 0; i < shapes_.size(); i++) {
            str.append("S");
            str.append(to_string(shapes_.at(i)->getId()));
            str.append(":");
            str.append(to_string(correspondingIds_.at(i)));
            if(i < shapes_.size()-1) {
                str.append("->");
            }
        }
        
        return str;
    }
};

#endif /* defined(__ShapeAnalyzer__FaceCorrespondence__) */
