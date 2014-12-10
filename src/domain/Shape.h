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
#include <vtkCommand.h>

#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include "PetscHelper.h"


using namespace std;

///
/// \brief The Shape contains all geometric information as well as VTK-related objects and basic information about the shape.
/// \details Every shape has a unique and static id and a not necessarily unique name which can be changed.
/// \author Emanuel Laude and Zorah Lähner
/// \note Documentation not finished.
///
class Shape {
public:
    enum class VisualRepresentation { MeshSurface, InterpolatedNormals, PointCloud, Mesh };
    
    /// \brief Coloring struct. Contains a type and the coloring data as vtkSmartPointer<vtkDataArray>.
    struct Coloring {
        /// \brief Type enum.
        enum class Type { PointSegmentation, FaceSegmentation, PointRgb, FaceRgb, PointScalar, FaceScalar };
        
        /// \brief Color data. It can be either point or face data.
        /// \details Moreover it is either an 1D (scalar) or an 3D (RGB) array.
        vtkSmartPointer<vtkDataArray> values;
        
        /// \brief Type of the coloring. It is either PointSegmentation, FaceSegmentation, PointRgb, FaceRgb, PointScalar or FaceScalar
        Type type;
    };
    
    /// Constructor.
    Shape(vtkIdType id, string name, vtkSmartPointer<vtkPolyData> polyData, vtkSmartPointer<vtkRenderer> renderer);
    
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
    
    // Getters
    
    /// \brief Returns the actor of the shape.
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }

    /// \brief Returns the mesh data.
    vtkSmartPointer<vtkPolyData> getPolyData() {
        return polyData_;
    }
    
    vtkLinearTransform* getTransformation() {
        return actor_->GetUserTransform();
    }
    
    vtkScalarsToColors* getLookupTable() {
        return mapper_->GetLookupTable();
    }
    
    ///@}
    
    /// \brief This function has to be called after polyData was modified.
    /// \details It rerenders the shape and updates the bounding box of the BoxWidget.
    void modified() {
        polyData_->Modified();
        actor_->Modified();
        
        static_cast<vtkBoxRepresentation*>(boxWidget_->GetRepresentation())->PlaceWidget(polyData_->GetBounds());
        static_cast<vtkBoxRepresentation*>(boxWidget_->GetRepresentation())->SetTransform((vtkTransform*) actor_->GetUserTransform());
    }
    
    /// \brief Adds an observer to the boxWidget. If the shape is transformed independently from the scene via the boxWidget the observers are executed.
    void addObserver(vtkSmartPointer<vtkCommand> callback) {
        boxWidget_->AddObserver(vtkCommand::InteractionEvent, callback);
    }
    
    /// \brief Transforms the shape
    /// @param vtkSmartPointer<vtkTransform> The transformation object.
    void transform(vtkSmartPointer<vtkTransform> t) {
        actor_->SetUserTransform(t);
    }
    
    void setVisualRepresentation(VisualRepresentation representation) {
        switch (representation) {
            case VisualRepresentation::MeshSurface:
                mapper_->SetInputData(polyData_);
                actor_->GetProperty()->SetRepresentationToSurface();
                actor_->GetProperty()->SetColor(1, 1, 1);
                actor_->Modified();
                
                break;
            case VisualRepresentation::InterpolatedNormals:
                mapper_->SetInputData(polyDataNormals_->GetOutput());
                actor_->GetProperty()->SetRepresentationToSurface();
                actor_->GetProperty()->SetColor(1, 1, 1);
                actor_->Modified();
                
                break;
                
            case VisualRepresentation::PointCloud:
                actor_->GetProperty()->SetPointSize(3);
                actor_->GetProperty()->SetRepresentationToPoints();
                actor_->GetProperty()->SetColor(0, 0, 1);
                actor_->Modified();
                
                break;
                
            case VisualRepresentation::Mesh:
                mapper_->SetInputData(polyData_);
                actor_->GetProperty()->SetRepresentationToWireframe();
                actor_->GetProperty()->SetColor(1, 1, 0);
                actor_->Modified();
                
                break;
            default:
                break;
        }
    }
    
    void setShowBoxWidget(bool showBoxWidget) {
        if(showBoxWidget) {
            boxWidget_->On();
        } else {
            boxWidget_->Off();
        }
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

    shared_ptr<const Coloring> getColoring() const {
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
    /// \brief Unique id of the shape
    /// \details Although the ShapeAnalyzer only assigns unique ids, the uniqueness is not
    /// forced unconditionally
    vtkIdType id_;
    /// Non-unique name of the shape
    string name_;
    
    /// @name VTK objects
    ///@{
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkPolyDataMapper> mapper_;
    vtkSmartPointer<vtkBoxWidget2> boxWidget_;
    vtkSmartPointer<vtkPolyData> polyData_;
    vtkSmartPointer<vtkPolyDataNormals> polyDataNormals_;
    
    vtkSmartPointer<vtkRenderer> renderer_;
    ///@}
    
    /// \brief Current Coloring of the shape, if there is any
    shared_ptr<Coloring> coloring_;
};

#endif