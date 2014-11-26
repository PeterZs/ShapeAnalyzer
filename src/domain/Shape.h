#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget2.h>
#include <vtkBoxRepresentation.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkGlyph3D.h>
#include <vtkIdList.h>
#include <vtkLookupTable.h>
#include <vtkLinearTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectBase.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTriangle.h>
#include <vtkType.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include "PetscHelper.h"

#include "../io/Serializable.h"


using namespace std;

///
/// \brief The Shape contains all geometric information as well as VTK-related objects and basic information about the shape.
/// \details Every shape has a unique and static id and a not necessarily unique name which can be changed.
/// \author Emanuel Laude and Zorah Lähner
/// \note Documentation not finished.
///
class Shape : public Serializable {
    
public:
    /// \brief Coloring struct. Contains a type and the coloring data as vtkSmartPointer<vtkDataArray>.
    struct Coloring {
        /// \brief Type enum.
        enum class Type {PointSegmentation, FaceSegmentation, PointRgb, FaceRgb, PointScalar, FaceScalar};
        
        /// \brief Color data. It can be either point or face data.
        /// \details Moreover it is either an 1D (scalar) or an 3D (RGB) array.
        vtkSmartPointer<vtkDataArray> values;
        
        /// \brief Type of the coloring. It is either PointSegmentation, FaceSegmentation, PointRgb, FaceRgb, PointScalar or FaceScalar
        Type type;
    };
    
    /// Constructor.
    Shape(vtkIdType id, string name, vtkSmartPointer<vtkPolyData> polyData, vtkSmartPointer<vtkRenderer> renderer);
    
    /// Constructor.
    Shape(vtkSmartPointer<vtkRenderer> renderer);
    
    /// Virtual destructor.
    virtual ~Shape() {
    }
    
    void initialize();
    
    /// \brief Returns the area of the shape.
    /// \details Calculates the area of every cell in the poly data and adds them up. Assumes that all cells are
    /// triangles, the results will be faulty for non-triangulated meshes.
    double getArea();
    /// \brief Returns the index of an random point on the shape.
    vtkIdType getRandomPoint();
    
    // vtk functions
    
    /// \brief Will remove the shape from the renderer. The shape will not be shown after the next rerender.
    void removeFromRenderer();
    /// \brief Removes any Scalar information that is used to color the shape and resets all properties of the mapper to default.
    void clearColoring();
    
    /// @name Serialization functions
    /// \brief Can write / read the shape into / from binary or ASCII format.
    ///@{
    
    virtual ostream& writeBinary(ostream& os);
    
    virtual ostream& writeASCII(ostream& os);
    
    virtual istream& readBinary(istream& is);
    
    virtual istream& readASCII(istream& is);
    
    ///@}
    
    // Getters
    
    
    /// \brief Returns the actor of the shape.
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    /// \brief Returns the box widget, that can be used to transform the shape independently from the scene.
    vtkSmartPointer<vtkBoxWidget2> getBoxWidget() {
        return boxWidget_;
    }
    
    /// \brief Returns the mesh data.
    vtkSmartPointer<vtkPolyData> getPolyData() {
        return polyData_;
    }
    
    /// \brief Returns the renderer.
    vtkSmartPointer<vtkRenderer> getRenderer() {
        return renderer_;
    }
    
    /// \brief Returns the smoothed version of the mesh data.
    vtkSmartPointer<vtkPolyDataNormals> getPolyDataNormals() {
        return polyDataNormals_;
    }
    
    /// \brief Returns the mapper of the shape.
    vtkSmartPointer<vtkPolyDataMapper> getMapper() {
        return mapper_;
    }
    
    
    /// \brief Returns the unique id of the shape.
    vtkIdType getId() {
        return id_;
    }
    
    /// \brief Returns the name of the shape.
    string getName() {
        return name_;
    }
    
    /// \brief Resets the name of the shape.
    void setName(string name) {
        name_ = name;
    }

    /// \brief Colors points or faces with any kind of Coloring.
    /// \details Depending on the type of the coloring different coloring schemas are applied.
    /// @param shared_ptr<Coloring>. Contains the color data for the points or faces and the type of the coloring.
    void setColoring(shared_ptr<Coloring> coloring);

    shared_ptr<Coloring> getColoring() {
        return coloring_;
    }

    /// \brief Colors the shape using the coordinates of the vertices as indicators for the rgb value.
    /// \details The values are the coordinates of each vertex but normalized to values between 0 and 255.
    void colorPointsCoordinates();

    /// \brief Colors the shape using the coordinates of the faces as indicators for the rgb value.
    /// \details The values are the interpolated values of the vertices of each face
    /// but normalized to values between 0 and 255. The class inherits from CoordinateColoring
    /// the only overwritten function is the calculation of the color for each face.
    void colorFacesCoordinates();

private:    
    vtkIdType id_;
    string name_;
    
    // vtk objects
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkPolyDataMapper> mapper_;
    vtkSmartPointer<vtkBoxWidget2> boxWidget_;
    vtkSmartPointer<vtkPolyData> polyData_;
    vtkSmartPointer<vtkPolyDataNormals> polyDataNormals_;
    
    vtkSmartPointer<vtkRenderer> renderer_;
    
    shared_ptr<Coloring> coloring_;
};

#endif