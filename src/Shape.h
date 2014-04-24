#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include <vector>

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
    
    vtkSmartPointer<vtkPolyData> getData() {
        return data_;
    }
    
    // setters
    void setActor(vtkSmartPointer<vtkActor> actor) {
        actor_ = actor;
    }
    
    void setBoxWidget(vtkSmartPointer<vtkBoxWidget> boxWidget) {
        boxWidget_ = boxWidget;
    }
    
    void setData(vtkSmartPointer<vtkPolyData> data) {
        data_ = data;
    }
    
private:
    vtkSmartPointer<vtkActor> actor_;
    vtkSmartPointer<vtkBoxWidget> boxWidget_;
    vtkSmartPointer<vtkPolyData> data_;

};

#endif