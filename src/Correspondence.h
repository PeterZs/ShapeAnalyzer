#ifndef Correspondence_H
#define Correspondence_H

#include <vtkActor.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>
#include <vtkLinearTransform.h>
#include <vtkProperty.h>

#include "Shape.h"

#include <vector>

using namespace std;

class Correspondence {

public:
    Correspondence(Shape* shape1, Shape* shape2, double* point1, double* point2, vtkLinearTransform* t1, vtkLinearTransform* t2);
    
    Correspondence(const Correspondence& c);
    
    ~Correspondence();
    
    //apply current user-transformation t to original point1_ and update line source with the transformed point
    void transformPoint1(vtkLinearTransform* t);
    
    //apply current user-transformation t to original point2_ and update line source with the transformed point
    void transformPoint2(vtkLinearTransform* t);
    
    // getters
    vtkSmartPointer<vtkActor> getActor() {
        return actor_;
    }
    
    double* getPoint1() {
        return point1_;
    }
    
    double* getPoint2() {
        return point2_;
    }
    
    Shape* getShape1() {
        return shape1_;
    }
    
    Shape* getShape2() {
        return shape2_;
    }
    
private:
    //private setters.
    void setPoint1(double* point) {
        point1_[0] = point[0];
        point1_[1] = point[1];
        point1_[2] = point[2];
    }
    
    void setPoint2(double* point) {
        point2_[0] = point[0];
        point2_[1] = point[1];
        point2_[2] = point[2];
    }
    
    vtkSmartPointer<vtkLineSource> line_;
    vtkSmartPointer<vtkActor> actor_;
    
    // point1 in the line source belongs to shape1 and the same with 2
    Shape* shape1_;
    Shape* shape2_;
    
    //point1 point2 hold the original 3d-coordinates of the corresponding points and NOT the transformed coordinates
    double* point1_;
    double* point2_;
};

#endif
