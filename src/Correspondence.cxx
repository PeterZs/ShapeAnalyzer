#include "Correspondence.h"

// Constructor
Correspondence::Correspondence(Shape* shape1, Shape* shape2, vtkSmartPointer<vtkTriangle> triangle1, vtkSmartPointer<vtkTriangle> triangle2) {

    triangle1_ = triangle1;
    triangle2_ = triangle2;
    
    
    triangleCenter1_ = new double[3];
    
    //initialize point1 to center of triangle1
    //method TriangleCenter is static therefore call it as vtkTriangle::TriangleCenter
    //For some strange reason stated in the vtk doc we have to use triangle1_->GetPoints()->GetPoint(0, p1) instead of triangle1_->GetPoints()->GetPoint(0)
    double p1[3];
    double p2[3];
    double p3[3];
    triangle1_->GetPoints()->GetPoint(0, p1);
    triangle1_->GetPoints()->GetPoint(1, p2);
    triangle1_->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, triangleCenter1_);
    
    triangleCenter2_ = new double[3];
    //initialize point2 to center of triangle2
    triangle2_->GetPoints()->GetPoint(0, p1);
    triangle2_->GetPoints()->GetPoint(1, p2);
    triangle2_->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, triangleCenter2_);

    
    shape1_ = shape1;
    shape2_ = shape2;
    
    line_ = vtkSmartPointer<vtkLineSource>::New();
    // set points in line source, transform if user transformation happend
    vtkLinearTransform* t1 = shape1->getActor()->GetUserTransform();
    if(t1 == nullptr) {
        line_->SetPoint1(triangleCenter1_);
    } else {
        transformTriangleCenter1(t1);
    }
    
    vtkLinearTransform* t2 = shape2->getActor()->GetUserTransform();
    if(t2 == nullptr) {
        line_->SetPoint2(triangleCenter2_);
    } else {
        transformTriangleCenter2(t2);
    }
    
    //create mapper and actor. mapper kind of maps the line data source to the line actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(line_->GetOutputPort());
    
    actor_ = vtkSmartPointer<vtkActor>::New();
    actor_->SetMapper(mapper);
    actor_->GetProperty()->SetLineWidth(1);
    actor_->GetProperty()->SetColor(0, 1, 0);
}

//destructor
Correspondence::~Correspondence() {
    delete [] triangleCenter1_;
    delete [] triangleCenter2_;
}

//copy constructor
Correspondence::Correspondence(const Correspondence& c) : line_(c.line_), actor_(c.actor_), shape1_(c.shape1_), shape2_(c.shape2_), triangle1_(c.triangle1_), triangle2_(c.triangle2_) {
    //point1 point2 must be reallocated and their values must be copied if Correspondence is copied. Each copy of Correspondence has to have its own point1 point2 allocated in memory
    triangleCenter1_ = new double[3];
    triangleCenter2_ = new double[3];
    
    triangleCenter1_[0] = c.triangleCenter1_[0];
    triangleCenter1_[1] = c.triangleCenter1_[1];
    triangleCenter1_[2] = c.triangleCenter1_[2];
    
    triangleCenter2_[0] = c.triangleCenter2_[0];
    triangleCenter2_[1] = c.triangleCenter2_[1];
    triangleCenter2_[2] = c.triangleCenter2_[2];
}

void Correspondence::transformTriangleCenter1(vtkLinearTransform* t) {
    line_->SetPoint1(t->TransformPoint(triangleCenter1_));
}

void Correspondence::transformTriangleCenter2(vtkLinearTransform* t) {
    line_->SetPoint2(t->TransformPoint(triangleCenter2_));
}
