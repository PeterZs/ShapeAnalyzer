#ifndef __ShapeAnalyzer__Correspondence__
#define __ShapeAnalyzer__Correspondence__

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkType.h>
#include <vtkLine.h>
#include <vtkLinearTransform.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>

#include "../../util/HashMap.h"
#include "../Shape.h"

#include "CorrespondenceData.h"

#include <vector>
#include <algorithm>

using namespace std;

///
/// \brief Abstract Class for Correspondences that are shown in the GUI.
///
/// \details Contains a CorrespondenceData Pointer and can visualize this data
/// in the GUI with lines and highlighted faces or points.
///
class Correspondence {

public:
    /// \brief Virtual destructor.
    virtual ~Correspondence() {
        actors_.clear();
    }
    
    /// \brief Initializes the internal data structures for visualization.
    /// \details In case the CorrespondenceData object provided as an constructor argument is not empty it adds all the corresponding actors and lines.
    void initialize();
    
    /// \brief Transforms this correspondence in case the shape provided as a parameter is part of it.
    /// \details To do so it applies the current user transformation of the shape to the original coordinates of the respective end point of the line representing the correspondence. Afterwards it updates the line with the transformed coordinates and sets the current user transformation of the actor representing the point or face of the transformed shape to the new one.
    /// @param Shape * shape. The shape that was transformed by the user.
    void transform(Shape* shape);
    
    
    /// \brief Highlights this correspondence in case it was selected.
    /// @param bool selected. Value is indicating whether the correspondence should be highlighted or not.
    void setSelected(bool selected);

    /// \brief Adds another shape plus face ID (or vertex ID) pair to this correspondence.
    /// \details Returns 1 if shape plus data id combination has successfully been added (shape has not been selected twice). Returns 0 if the shape equals the last added shape and updates the  coordinates of the lines end point with the new coordinates. Returns -1 if the shape is identical to another shape that has already been added before and the shape is not equals to the last added shape.
    int addShape(Shape* shape, vtkIdType);
    
    /// \brief Removes this shape from the Correspondence, if it was a part of it.
    /// \details The actor is updated and the vtk widget rendered, if necessary.
    /// @return 1 if the shape was removed...
    /// \note TODO
    int removeShape(Shape* shape);
    

    /// Returns the actor representing the line
    vtkActor* getLinesActor() {
        return linesActor_;
    }
    
    /// Returns a vector of all shapes that are part of this correspondence.
    vector<Shape*>& getShapes() {
        return shapes_;
    }
    
    /// Returns the CorrespondenceData object.
    CorrespondenceData* getData() {
        return data_;
    }
    
    /// Removes the correspondence from the renderer provided as a constructor argument.
    void removeFromRenderer();
    
    /// Adds the correspondence to the renderer provided as a contructor argument.
    void addToRenderer();
protected:
    /// \brief Protected contructor since class is abstract. Empty CorrespondenceData. Call initialize afterwards.
    /// @param vtkSmartPointer<vtkRenderer> renderer. The renderer to which the correspondence is added afterwards using the function addToRenderer().
    /// @param CorrespondenceData * data. An empty CorrespondenceData object.
    
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data);
    /// \brief Protected contructor since class is abstract. To call when the CorrespondenceData is not empty. Call initialize afterwards.
    /// @param vtkSmartPointer<vtkRenderer> renderer. The renderer to which the correspondence is added afterwards using addToRenderer().
    /// @param CorrespondenceData * data. A CorrespondenceData object containing the actual correspondence data (vector of shape IDs and point or face IDs).
    /// @param HashMap<vtkActor*, Shape *>& shapes. A list of shapes. Needed to get the actual Shape object from the shapeID provided via the CorrespondenceData object.
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data, HashMap<vtkActor*, Shape*>& shapes);
    
    /// \brief Abstract protected function that initializes the actor representing either a face or a point correspondence. Implemented by FaceCorrespondence or PointCorrespondence.
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType) = 0;
    
    /// \brief Abstract protected function that returns the coordinates of the lines end point given a Shape and a point or vertex ID. Implemented by FaceCorrespondence or PointCorrespondence.
    /// \details FaceCorrespondence returns the center of the triangle face as an end point. PointCorrespondence simply returns the coordinates of the point. This function is used internally by the functions initialize() or addShape() to construct the correspondence lines.
    /// @param double point[3]. The 3D coordinates of the point that is returned.
    /// @param Shape * shape. The shape on which the point is located.
    /// @param vtkIdType id. The point or face ID.
    virtual void getCorrespondencePoint(double point[3], Shape* shape, vtkIdType) = 0;
    
    /// \brief Renderer.
    vtkSmartPointer<vtkRenderer>    renderer_;
    /// \brief The CorrespondenceData this object is showing.
    CorrespondenceData*             data_;
    /// \brief Points to all shapes in the correspondence.
    /// \details The order in the vector is the same as the one that can be received from
    /// data_.
    vector<Shape*>                  shapes_;

    
    ///@{
    
    /// \brief Actors highlighting the data (vertices of faces) on the corresponding shapes
    /// \details The order correspondes to the order of the shapes in the CorrespondenesData
    /// data_ and shapes_.
    vector<vtkSmartPointer<vtkActor> >  actors_;
    
    /// \brief Reference coordinates for the lines.
    /// \details The coordinates are used to transfrom the lines if the shape is transformed
    /// independently of the scene.
    vtkSmartPointer<vtkPoints>          lineReferencePoints_;

    /// \brief PolyData containing the end points of the lines.
    /// \details The points are ordered in the same way as the shapes in data_ and shapes_, i.e.
    /// the point with id i in this object correspondes to the shape on position i in shapes_.
    vtkSmartPointer<vtkPolyData>        linesPolyData_;
    /// \brief Mapper of the lines and highlightings.
    vtkSmartPointer<vtkPolyDataMapper>  linesMapper_;
    /// \brief Actors of the lines.
    vtkSmartPointer<vtkActor>           linesActor_;
    
    ///@}
};

#endif /* defined(__ShapeAnalyzer__Correspondence__) */
