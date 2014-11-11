#ifndef FaceCorrespondenceData_H
#define FaceCorrespondenceData_H

#include "CorrespondenceData.h"
///
/// \brief Stores multi correspondences between faces of an arbitrary number of shapes.
/// \details The main implementation can be seen in CorrespondenceData, this class
/// purely insures that point and face correspondences can not be mixed up in some
/// data structures (especially faceCorrespondencesData_ in ShapeAnalyzer).
///
class FaceCorrespondenceData : public CorrespondenceData {
public:
    /// \brief Basic Constructor.
    /// @param id The final id of this correspondence data. It should be unique
    /// among all correspondence data, but this is not enforced.
    FaceCorrespondenceData(vtkIdType id) : CorrespondenceData(id) {
    }
    
    /// \brief Returns "FaceCorrespondenceData".
    virtual string getType() {
        return "FaceCorrespondenceData";
    }
};

#endif /* defined(FaceCorrespondenceData_H) */
