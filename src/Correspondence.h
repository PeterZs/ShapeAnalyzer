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
    Correspondence(Shape* shape1, Shape* shape2, vtkSmartPointer<vtkTriangle> face1, vtkSmartPointer<vtkTriangle> face2, vtkSmartPointer<vtkActor> face1Actor, vtkSmartPointer<vtkActor> face2Actor);
    
    //apply current user-transformation t to original face center point1_ and update line source with the transformed point
    void transformPoint1(vtkLinearTransform* t);
    
    //apply current user-transformation t to original face center point2_ and update line source with the transformed point
    void transformPoint2(vtkLinearTransform* t);
    
    // getters
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    vtkSmartPointer<vtkTriangle> getFace1() {
        return face1_;
    }
    
    vtkSmartPointer<vtkTriangle> getFace2() {
        return face2_;
    }

    vtkSmartPointer<vtkActor> getFace1Actor() {
        return face1Actor_;
    }
    
    vtkSmartPointer<vtkActor> getFace2Actor() {
        return face2Actor_;
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

    // point1 and face1 in the line source belongs to shape1 and the same with 2
    vtkSmartPointer<vtkTriangle> face1_;
    vtkSmartPointer<vtkTriangle> face2_;
    vtkSmartPointer<vtkActor> face1Actor_;
    vtkSmartPointer<vtkActor> face2Actor_;
    double point1_[3];
    double point2_[3];
};

#endif
