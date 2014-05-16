#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkIdList.h>
#include <vtkLinearTransform.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkType.h>
#include <vtkVertexGlyphFilter.h>

using namespace std;

class Shape {
public:
    Shape(vtkIdType                     shapeID,
          vtkSmartPointer<vtkPolyData>  polyData,
          vtkSmartPointer<vtkPolyData>  polyDataNormals,
          vtkSmartPointer<vtkRenderer>  renderer);

    Shape();
    ~Shape() { };
    
    void       visualizeEuclidean(int start = -1);
    double     getEuclideanDistances(int start, std::vector<double> &result);
    
    vtkSmartPointer<vtkIdList>  getFPS(unsigned numberSamples, int start = -1);
    void                        setFPS(unsigned numberSamples, int start = -1);
    void                        transformFPS(vtkLinearTransform* t);
    
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
    
    vtkSmartPointer<vtkActor> getFPSActor() {
        return fpsActor_;
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