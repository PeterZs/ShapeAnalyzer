#ifndef Correspondence_H
#define Correspondence_H

#include <vtkActor.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>
#include <vtkLinearTransform.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkRenderer.h>

#include "Shape.h"
#include "CorrespondenceData.h"

#include <vector>

using namespace std;

class Correspondence {

public:
    virtual ~Correspondence() {
        delete data_;
    };
    
    //apply current user transformation t to original point1_ and update line source with the transformed point
    void transform1(vtkLinearTransform* t);
    
    //apply current user transformation t to original point2_ and update line source with the transformed point
    void transform2(vtkLinearTransform* t);
    
    // getters
    Shape* getShape1() {
        return shape1_;
    }
    
    Shape* getShape2() {
        return shape2_;
    }
    
    vtkSmartPointer<vtkActor> getLineActor() {
        return lineActor_;
    }
    
    void setSelected(bool selected);
    
    void remove();
protected:
    //protected contructor
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, Shape* shape1, Shape* shape2, vtkSmartPointer<vtkActor> actor1, vtkSmartPointer<vtkActor> actor2);
    
    
    void visualize(double point1[3], double point[3]);

    vtkSmartPointer<vtkRenderer> renderer_;
    
    CorrespondenceData* data_;
    
private:
    //vtk visualization stuff
    vtkSmartPointer<vtkPolyData> linePolyData_; //line polydata that is visualized
    vtkSmartPointer<vtkActor> lineActor_; // line actor
    
    vtkSmartPointer<vtkActor> actor1_;
    vtkSmartPointer<vtkActor> actor2_;
    

    Shape* shape1_;
    Shape* shape2_;

    double point1_[3];
    double point2_[3];
};

#endif
