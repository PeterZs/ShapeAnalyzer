#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkIdList.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkType.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkRenderWindow.h>

using namespace std;

class Shape {
public:
    Shape(vtkIdType                     shapeID,
          vtkSmartPointer<vtkPolyData>  polyData,
          vtkSmartPointer<vtkPolyData>  polyDataNormals,
          vtkSmartPointer<vtkRenderer>  renderer);

    Shape();
    ~Shape() { };
    
    vtkSmartPointer<vtkIdList> getFPS(unsigned numberSamples);
    vtkSmartPointer<vtkIdList> getFPS(unsigned numberSamples, unsigned start);
    
    // getters
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    vtkSmartPointer<vtkBoxWidget> getBoxWidget() {
        return boxWidget_;
    }
    
    vtkSmartPointer<vtkIdList> getFPS() {
        return fps_;
    }
    
    vtkSmartPointer<vtkPolyData> getPolyData() {
        return polyData_;
    }

    vtkSmartPointer<vtkPolyData> getPolyDataNormals() {
        return polyDataNormals_;
    }
    
    vtkSmartPointer<vtkPolyDataMapper> getMapper() {
        return mapper_;
    }
    
    vtkIdType getId() {
        return shapeId_;
    }
    
    void remove();
    
private:
    vtkIdType shapeId_;
    
    vtkSmartPointer<vtkActor>           actor_;
    vtkSmartPointer<vtkBoxWidget>       boxWidget_;
    vtkSmartPointer<vtkPolyDataMapper>  mapper_;
    vtkSmartPointer<vtkPolyData>        polyData_;
    vtkSmartPointer<vtkPolyData>        polyDataNormals_;
    vtkSmartPointer<vtkRenderer>        renderer_;
    
    vtkSmartPointer<vtkIdList>          fps_;
    vtkSmartPointer<vtkActor>           fpsActor_;

};

#endif