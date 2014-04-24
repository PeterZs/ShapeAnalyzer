#include "Correspondence.h"

// Constructor
Correspondence::Correspondence(Shape* shape1, Shape* shape2, double* point1, double* point2, vtkLinearTransform* t1, vtkLinearTransform* t2) {
    
    point1_ = new double[3];
    point2_ = new double[3];
    
    setPoint1(point1);
    setPoint2(point2);
    shape1_ = shape1;
    shape2_ = shape2;
    
    line_ = vtkSmartPointer<vtkLineSource>::New();
    // set points in line source, transform if user transformation happend
    if(t1 == nullptr) {
        line_->SetPoint1(point1_);
    } else {
        transformPoint1(t1);
    }

    if(t2 == nullptr) {
        line_->SetPoint2(point2_);
    } else {
        transformPoint2(t2);
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
    delete [] point1_;
    delete [] point2_;
}

//copy constructor
Correspondence::Correspondence(const Correspondence& c) : line_(c.line_), actor_(c.actor_), shape1_(c.shape1_), shape2_(c.shape2_) {
    //point1 point2 must be reallocated and their values must be copied if Correspondence is copied. Each copy of Correspondence has to have its own point1 point2 allocated in memory
    point1_ = new double[3];
    point2_ = new double[3];
    
    point1_[0] = c.point1_[0];
    point1_[1] = c.point1_[1];
    point1_[2] = c.point1_[2];
    
    point2_[0] = c.point2_[0];
    point2_[1] = c.point2_[1];
    point2_[2] = c.point2_[2];
}

void Correspondence::transformPoint1(vtkLinearTransform* t) {
    line_->SetPoint1(t->TransformPoint(point1_));
}

void Correspondence::transformPoint2(vtkLinearTransform* t) {
    line_->SetPoint2(t->TransformPoint(point2_));
}
