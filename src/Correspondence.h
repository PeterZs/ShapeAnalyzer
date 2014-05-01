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

#include "Shape.h"

#include <vector>

using namespace std;

class Correspondence {

public:
    Correspondence(Shape* shape1, Shape* shape2, vtkSmartPointer<vtkTriangle> triangle1, vtkSmartPointer<vtkTriangle> triangle2, vtkSmartPointer<vtkActor> triangle1Actor, vtkSmartPointer<vtkActor> triangle2Actor);
    
    //apply current user-transformation t to original triangle center point1_ and update line source with the transformed point
    void transformPoint1(vtkLinearTransform* t);
    
    //apply current user-transformation t to original triangle center point2_ and update line source with the transformed point
    void transformPoint2(vtkLinearTransform* t);
    
    // getters
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    vtkSmartPointer<vtkTriangle> getTriangle1() {
        return triangle1_;
    }
    
    vtkSmartPointer<vtkTriangle> getTriangle2() {
        return triangle2_;
    }

    vtkSmartPointer<vtkActor> getTriangle1Actor() {
        return triangle1Actor_;
    }
    
    vtkSmartPointer<vtkActor> getTriangle2Actor() {
        return triangle2Actor_;
    }
    
    Shape* getShape1() {
        return shape1_;
    }
    
    Shape* getShape2() {
        return shape2_;
    }
private:
    //vtk visualization stuff
    vtkSmartPointer<vtkPolyData> polyData_; //polydata that is visualized
    vtkSmartPointer<vtkActor> actor_;
    
    Shape* shape1_;
    Shape* shape2_;

    // point1 and triangle1 in the line source belongs to shape1 and the same with 2
    vtkSmartPointer<vtkTriangle> triangle1_;
    vtkSmartPointer<vtkTriangle> triangle2_;
    vtkSmartPointer<vtkActor> triangle1Actor_;
    vtkSmartPointer<vtkActor> triangle2Actor_;
    double point1_[3];
    double point2_[3];
};

#endif
