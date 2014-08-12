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

#include "LaplaceBeltramiOperator.h"
#include "FEMLaplaceBeltramiOperator.h"
#include "attributes/ScalarPointAttribute.h"
#include "PetscHelper.h"


class ScalarPointAttribute;

using namespace std;


class Shape {

public:
    // Constructors and Destructor
    Shape(vtkIdType shapeID, vtkSmartPointer<vtkPolyData> polyData, vtkSmartPointer<vtkRenderer> renderer);
    Shape(vtkSmartPointer<vtkRenderer> renderer);
    ~Shape() {
        delete laplacian_;
    }
    
    double                      calculateArea();
    vtkIdType                    getRandomPoint();
    
    // vtk functions
    
    void remove();
    
    // serialization functions
    
    ostream& write(ostream& os);
    
    friend ostream& operator<<(ostream& os, const Shape& shape);
    
    istream& read(istream& is);
    
    friend istream& operator>>(istream& is, Shape& shape);
    
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
        return shapeId_;
    }

    LaplaceBeltramiOperator* getLaplacian(int numberOfEigenfunctions);
    
    void getEigenfunction(int i, ScalarPointAttribute& phi);
    
    double getEigenvalue(int i);
private:
    void initialize();
    
    vtkIdType shapeId_;
    
    // vtk objects
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkPolyDataMapper> mapper_;
    vtkSmartPointer<vtkBoxWidget> boxWidget_;
    vtkSmartPointer<vtkPolyData> polyData_;
    vtkSmartPointer<vtkPolyDataNormals> polyDataNormals_;
    
    vtkSmartPointer<vtkRenderer>        renderer_;

    LaplaceBeltramiOperator* laplacian_;
};

#endif