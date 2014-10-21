///
/// \brief Stores multi correspondences between points of an arbitrary number of shapes.
/// \details The main implementation can be seen in CorrespondenceData, this class
/// purely insures that point and face correspondences can not be mixed up in some
/// data structures (especially pointCorrespondenceData_ in ShapeAnalyzer).
///

#ifndef PointCorrespondeceData_H
#define PointCorrespondeceData_H

#include "CorrespondenceData.h"

class PointCorrespondenceData : public CorrespondenceData {
public:
    /// \brief Basic Constructor.
    /// @param id The final id of this correspondence data. It should be unique
    /// among all correspondence data, but this is not enforced.
    PointCorrespondenceData(vtkIdType id) : CorrespondenceData(id) {
    }
    
    /// \brief Returns "PointCorrespondenceData".
    virtual string getType() {
        return "PointCorrespondenceData";
    }
};

#endif /* defined(PointCorrespondeceData_H) */
