#ifndef Correspondence_H
#define Correspondence_H

#include <vtkActor.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>


#include <vector>

using namespace std;

class Correspondence {

	public: 
		Correspondence();
		~Correspondence() {};

	private: 
        vtkSmartPointer<vtkActor>		line_;
		vtkSmartPointer<vtkLineSource>	correspondence_;
		vtkSmartPointer<vtkIdType>		source_;
		vtkSmartPointer<vtkIdType>		target_;
		
};

#endif
