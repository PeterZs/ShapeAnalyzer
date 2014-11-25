#ifndef ShapeAnalyzer_ShapeAnalyzerInterface_h
#define ShapeAnalyzer_ShapeAnalyzerInterface_h

#include <vector>

#include "../domain/correspondences/CorrespondenceData.h"
#include "../domain/Shape.h"

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

/// \brief Interface declaring all public functions accessable by CustomTab or CustomContextMenuItem objects. All of them rerender the vtkWidget.
/// \details Implemented by ShapeAnalyzer. Read access to all shapes and all correspondences is
/// provided via the protected read-only (const) class members shapes_, pointCorrespondences_ and
/// faceCorrespondences_ in class CustomTab. Therefore only concrete CustomTab classes can have
/// access to all correspondences and all shapes at the same time.
/// \author Emanuel Laude and Zorah Lähner
///
class ShapeAnalyzerInterface {
public:
    /// \brief Adds and visualizes correspondence.
    /// \details Creates a new Correspondence and CorrespondenceData object visualizes it inserts an entry into the internal data structures and returns the created CorrespondenceData object.
    /// @param const vector<Shape*, vtkIdType>& correspondence. vector of Shape plus face or point ID pairs that are part of the correspondence.
    /// @param const type_info& type. Can either be PointCorrespondenceData or FaceCorrespondenceData
    virtual CorrespondenceData* addCorrespondence(const vector<pair<Shape*, vtkIdType>>& correspondence, const type_info& type) = 0;
    
    /// \brief Adds and visualizes a new shape.
    /// \details Creates a new Shape object given a name and the mesh data and returns the created Shape object.
    /// @param string name. The name of the shape.
    /// @param vtkSmartPointer<vtkPolyData> polyData. The mesh data.
    virtual Shape* addShape(string name, vtkSmartPointer<vtkPolyData> polyData) = 0;
    
    /// \brief Rerenders the vtkWidget scene.
    /// \details Any changes made for example by CustomTab classes to Correspondences or Shapes are not instantly visible.
    /// Use this function after all changes are made to update the visualisation when you are not using
    /// any functions of ShapeAnalyzer to make the changes. (i.e. deleteCorrespondence will rerender by itself)
    virtual void render() = 0;
};

#endif
