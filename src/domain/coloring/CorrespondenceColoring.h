///
/// \brief Colors the given shapes to visualize the given correspondences.
/// \details The reference shape will be colored by coordinates using the CoordinateColoring class and
/// all points with a corresponding point on another shape will be colored
/// in the same way as the correspondence point on the reference shape.
///
/// Additionally it is possible to keep track of how many and which points are actually corresponding
/// and if they are corresponding to multiple points on the reference shape.
///
/// \author Emanuel Laude and Zorah Lähner
///
///

#ifndef __ShapeAnalyzer__CorrespondenceColoring__
#define __ShapeAnalyzer__CorrespondenceColoring__

#include "CoordinateColoring.h"
#include "FaceCoordinateColoring.h"

#include <vtkSmartPointer.h>
#include <vtkType.h>
#include <vtkUnsignedCharArray.h>

#include "../Shape.h"
#include "../HashMap.h"

#include "../correspondences/FaceCorrespondence.h"
#include "../correspondences/PointCorrespondenceData.h"

#include "../attributes/ScalarPointAttribute.h"
#include "../attributes/ScalarFaceAttribute.h"

using namespace std;

class CorrespondenceColoring {
public:
    /// \brief Constructor.
    /// @param set HashMap of all shapes that will be colored.
    /// @param points HashMap of all PointCorrespondences that will be considered
    /// @param faces HashMap of all FaceCorrespondences that will be considered
    /// @param reference Optional. Pointer to shape that will be the reference. The first shape
    /// will be chosen if no reference is given.
    CorrespondenceColoring(HashMap<vtkActor*, Shape*>*              set,
                           HashMap<PointCorrespondenceData*, bool>* points,
                           HashMap<FaceCorrespondenceData*, bool>*  faces,
                           Shape* reference = 0);
    
    /// \brief Visualizes the point correspondences going out from the reference shape.
    /// \details The reference shape will be colored by coordinates using the CoordinateColoring class and
    /// all points with a corresponding point on another shape will be colored
    /// in the same way as the correspondence point on the reference shape. If a vertex is part of multiple
    /// correspondences, an arbitrary correspondence is visualized.
    /// The parameters can be used to get information of the density of the correspondence between two
    /// shapes and the percentage of doubly matched points.
    /// @param percentageMatched Optional. For every shape that is not the reference shape - identified
    /// by the shape id - the vector will contain the percentage of its vertices that are mapped (might be multiple times)
    /// to any vertex of the reference shape.
    /// @param percentageMultiple Optional. For every shape that is not the reference shape - identified
    /// by the shape id - the vector will contain the percentage of its vertices that are mapped to a vertex
    /// on the reference shape multiple times.
    void showPointCorrespondences(vector<pair<vtkIdType, double> >* percentageMatched = 0,
                                  vector<pair<vtkIdType, double> >* percentageMultiple = 0);
    /// \brief Visualizes the face correspondences going out from the reference shape.
    /// \details The reference shape will be colored by coordinates using the CoordinateColoring class and
    /// all points with a corresponding face on another shape will be colored
    /// in the same way as the correspondence face on the reference shape. If a face is part of multiple
    /// correspondences, an arbitrary correspondence is visualized.
    /// The parameters can be used to get information of the density of the correspondence between two
    /// shapes and the percentage of doubly matched points.
    /// @param percentageMatched Optional. For every shape that is not the reference shape - identified
    /// by the shape id - the vector will contain the percentage of its faces that are mapped (might be multiple times)
    /// to any face of the reference shape.
    /// @param percentageMultiple Optional. For every shape that is not the reference shape - identified
    /// by the shape id - the vector will contain the percentage of its faces that are mapped to a face
    /// on the reference shape multiple times.
    void showFaceCorrespondences(vector<pair<vtkIdType, double> >* percentageMatched = 0,
                                 vector<pair<vtkIdType, double> >* percentageMultiple = 0);
    
protected:
    HashMap<vtkActor*, Shape*>*                 shapes_;
    HashMap<PointCorrespondenceData*, bool>*    points_;
    HashMap<FaceCorrespondenceData*, bool>*     faces_;
    
    Shape* reference_;
    
    unordered_map<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >  pointAttributes_;
    unordered_map<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >  faceAttributes_;
    
};

#endif /* defined(__ShapeAnalyzer__CorrespondenceColoring__) */
