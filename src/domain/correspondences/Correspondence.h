///
/// \brief Abstract Class for Correspondences that are shown in the GUI.
///
/// \details Contains a CorrespondenceData Pointer and can visualize this data
/// in the GUI with lines and highlighted faces.
/// \note TODO not finished
///

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

#include "../HashMap.h"
#include "../Shape.h"

#include "CorrespondenceData.h"

#include <vector>
#include <algorithm>

using namespace std;

class Correspondence {

public:
    virtual ~Correspondence() {
        actors_.clear();
    }
    
    void initialize();
    
    //apply current user transformation of shape to the reference points that belong to the transformed shape and update line source with the transformed point as well as the actors belonging to the corresponding faces or points.
    void transform(Shape* shape);
    
    void setSelected(bool selected);

    //add another shape + face ID (or vertex ID) pair to this correspondence returns 1 if shape id combi has successfully been added (i.e. shape has not been selected twice). Returns 0 if shape equals last added shape and updates coordinates. Returns -1 if shape equals another shape that has already been added and is not equals to last added shape
    int addShape(Shape* shape, vtkIdType);
    
    void addShapes(unordered_map<Shape*, vtkIdType>& shapes);
    
    // getters
    vtkActor* getLinesActor() {
        return linesActor_;
    }
    
    vector<Shape*>& getShapes() {
        return shapes_;
    }
    
    CorrespondenceData* getData() {
        return data_;
    }
    
    // vtk
    void removeFromRenderer();
    void addToRenderer();
    
protected:
    /// protected contructor since class is abstract
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data);
    /// protected contructor since class is abstract
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, CorrespondenceData* data, HashMap<vtkActor*, Shape*>& shapes);
    
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType) = 0;
    
    /// \note TODO
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
