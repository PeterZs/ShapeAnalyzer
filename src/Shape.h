#ifndef Shape_H
#define Shape_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include <vector>

using namespace std;

class Shape {

	public: 
		Shape();
		~Shape() {};

		// getters
		vtkSmartPointer<vtkActor> 		getActor() 		{ return actor_; };
		vtkSmartPointer<vtkBoxWidget> 	getBoxWidget() 	{ return boxWidget_; };
		vtkSmartPointer<vtkPolyData>	getData() 		{ return data_; };
		vtkSmartPointer<vtkPoints>		getPoints() 	{ return points_; };

		// setters
		void setActor(vtkSmartPointer<vtkActor> pActor) 	
							{ actor_ = pActor; };
		void setBoxWidget(vtkSmartPointer<vtkBoxWidget> pBoxWidget)
							{ boxWidget_ = pBoxWidget; };
		void setData(vtkSmartPointer<vtkPolyData> pData) 	
							{ data_ = pData; };
		void setPoints(vtkSmartPointer<vtkPoints> pPoints)	
							{ points_ = pPoints; };

	private: 
		vtkSmartPointer<vtkActor> 		actor_;
		vtkSmartPointer<vtkBoxWidget> 	boxWidget_;
		vtkSmartPointer<vtkPolyData>	data_;
		vtkSmartPointer<vtkPoints>		points_;

};

#endif