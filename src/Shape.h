#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

using namespace std;

class Shape {
public:
    Shape();
    ~Shape() {
    };
    
    // getters
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    vtkSmartPointer<vtkBoxWidget> getBoxWidget() {
        return boxWidget_;
    }
    
    vtkSmartPointer<vtkPolyData> getPolyData() {
        return polyData_;
    }

    vtkSmartPointer<vtkPolyData> getPolyDataNormals() {
        return polyDataNormals_;
    }
    
    // setters
    void setActor(vtkSmartPointer<vtkActor> actor) {
        actor_ = actor;
    }
    
    void setBoxWidget(vtkSmartPointer<vtkBoxWidget> boxWidget) {
        boxWidget_ = boxWidget;
    }
    
    void setPolyData(vtkSmartPointer<vtkPolyData> polyData) {
        polyData_ = polyData;
    }

    void setPolyDataNormals(vtkSmartPointer<vtkPolyData> polyDataNormals) {
        polyDataNormals_ = polyDataNormals;
    }
    
private:
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkBoxWidget> boxWidget_;
    vtkSmartPointer<vtkPolyData> polyData_;
    vtkSmartPointer<vtkPolyData> polyDataNormals_;

};

#endif