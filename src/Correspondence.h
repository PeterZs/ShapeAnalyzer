#ifndef Correspondence_H
#define Correspondence_H

#include <vtkActor.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>

#include "Shape.h"

#include <vector>

using namespace std;

class Correspondence {

	public: 
		Correspondence();
		~Correspondence() {};

		void updateLine();

		// getters
		vtkSmartPointer<vtkActor> 	getActor() 		{ return actor_; };
        double* 					getPoint1() 	
        								{ return correspondence_->GetPoint1(); };
        double* 					getPoint2() 	
        								{ return correspondence_->GetPoint2(); };
		Shape 						getShape1() 	{ return shape1_; };
		Shape 						getShape2() 	{ return shape2_; };

		// setters
		void setActor(vtkSmartPointer<vtkActor> pActor) 
					{
                        actor_ = pActor;
					};
		void setCorrespondence(vtkSmartPointer<vtkLineSource> pCorrespondence)
					{
						correspondence_ = pCorrespondence;
					};
        void setPoint1(double* pPoint)
					{
                        correspondence_->SetPoint1(pPoint);
					};
        void setPoint2(double* pPoint)
					{
                        correspondence_->SetPoint2(pPoint);
					};
		void setShape1(Shape pShape1) 
					{
						shape1_ = pShape1;
					};
		void setShape2(Shape pShape2) 
					{
						shape2_ = pShape2;
					};

	private: 
		vtkSmartPointer<vtkLineSource>		correspondence_;
        vtkSmartPointer<vtkActor>			actor_;
		// point1 in the line source belongs to shape1 and the same with 2
		Shape								shape1_;
		Shape								shape2_;
		
};

#endif
