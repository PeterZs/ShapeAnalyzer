#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkGlyph3D.h>
#include <vtkIdList.h>
#include <vtkLookUpTable.h>
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

#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include "PetscHelper.h"

#include "io/Serializable.h"



#include <vtkDebugLeaks.h>

using namespace std;


class Shape : public Serializable {

public:
    // Constructors and Destructor
    Shape(vtkIdType id, string name, vtkSmartPointer<vtkPolyData> polyData, vtkSmartPointer<vtkRenderer> renderer);
    Shape(vtkSmartPointer<vtkRenderer> renderer);
    virtual ~Shape() {
    }
    
    void initialize();
    
    double getArea();
    vtkIdType getRandomPoint();
    
    // vtk functions
    
    void removeFromRenderer();
    
    // serialization functions
    
    virtual ostream& writeBinary(ostream& os);
    
    virtual ostream& writeASCII(ostream& os);
    
    virtual istream& readBinary(istream& is);
    
    virtual istream& readASCII(istream& is);
    
    // Getters
    
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    vtkSmartPointer<vtkBoxWidget> getBoxWidget() {
        return boxWidget_;
    }
    
    vtkSmartPointer<vtkPolyData> getPolyData() {
        return polyData_;
    }
    
    vtkSmartPointer<vtkRenderer> getRenderer() {
        return renderer_;
    }
    
    vtkSmartPointer<vtkPolyDataNormals> getPolyDataNormals() {
        return polyDataNormals_;
    }
    
    vtkSmartPointer<vtkPolyDataMapper> getMapper() {
        return mapper_;
    }
    
    vtkIdType getId() {
        return id_;
    }
    
    string getName() {
        return name_;
    }
    
    void setName(string name) {
        name_ = name;
    }

private:
    
    vtkIdType id_;
    string name_;
    
    // vtk objects
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkPolyDataMapper> mapper_;
    vtkSmartPointer<vtkBoxWidget> boxWidget_;
    vtkSmartPointer<vtkPolyData> polyData_;
    vtkSmartPointer<vtkPolyDataNormals> polyDataNormals_;
    
    vtkSmartPointer<vtkRenderer>        renderer_;
};

#endif