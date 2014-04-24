#ifndef Correspondence_H
#define Correspondence_H

#include <vtkActor.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>
#include <vtkLinearTransform.h>
#include <vtkProperty.h>

#include "Shape.h"

#include <vector>

using namespace std;

class Correspondence {

public:
    Correspondence(Shape* shape1, Shape* shape2, vtkSmartPointer<vtkTriangle> triangle1, vtkSmartPointer<vtkTriangle> triangle2);
    
    Correspondence(const Correspondence& c);
    
    ~Correspondence();
    
    //apply current user-transformation t to original triangle center point1_ and update line source with the transformed point
    void transformTriangleCenter1(vtkLinearTransform* t);
    
    //apply current user-transformation t to original triangle center point2_ and update line source with the transformed point
    void transformTriangleCenter2(vtkLinearTransform* t);
    
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
    
    Shape* getShape1() {
        return shape1_;
    }
    
    Shape* getShape2() {
        return shape2_;
    }
private:
    
    vtkSmartPointer<vtkLineSource> line_;
    vtkSmartPointer<vtkActor> actor_;
    
    // point1 in the line source belongs to shape1 and the same with 2
    Shape* shape1_;
    Shape* shape2_;

    vtkSmartPointer<vtkTriangle> triangle1_;
    vtkSmartPointer<vtkTriangle> triangle2_;
    double* triangleCenter1_;
    double* triangleCenter2_;
};

#endif
