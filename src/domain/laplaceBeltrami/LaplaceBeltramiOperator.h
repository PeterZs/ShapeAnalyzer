//
//  LaplaceBeltramiOperator.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#ifndef ShapeAnalyzer_LaplaceBeltramiOperator_h
#define ShapeAnalyzer_LaplaceBeltramiOperator_h

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkDoubleArray.h>


#include "../Shape.h"

namespace laplaceBeltrami {
    
    class LaplaceBeltramiOperator {
    public:
        LaplaceBeltramiOperator(Shape* shape, int numberOfEigenfunctions) : shape_(shape), numberOfEigenfunctions_(numberOfEigenfunctions) {
        }
        
        virtual ~LaplaceBeltramiOperator() {
        }
        
        virtual vtkSmartPointer<vtkDoubleArray> getEigenfunction(int i) = 0;
        
        virtual double getEigenvalue(int i) = 0;
        
        int getNumberOfEigenfunctions() {
            return numberOfEigenfunctions_;
        }
    protected:
        Shape* shape_; //the shape
        int numberOfEigenfunctions_;
    };
    
}

#endif
