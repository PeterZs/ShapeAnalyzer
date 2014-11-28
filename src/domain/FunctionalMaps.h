//
//  FunctionalMaps.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 02.08.14.
//
//

#ifndef __ShapeAnalyzer__FunctionalMaps__
#define __ShapeAnalyzer__FunctionalMaps__

#include "Shape.h"

#include <vtkDoubleArray.h>

#include <vector>

using namespace std;

class FunctionalMaps {
public:
    FunctionalMaps(Shape* shape1, Shape* shape2, vector<vtkSmartPointer<vtkDoubleArray>>& c1, vector<vtkSmartPointer<vtkDoubleArray>>& c2, int numberOfEigenfunctions) : shape1_(shape1), shape2_(shape2), c1_(c1), c2_(c2), numberOfEigenfunctions_(numberOfEigenfunctions) {
        numberOfConstraints_ = c1_.size();
        
        if(c1_.size() != c2_.size()) {
            throw new invalid_argument("Number of elements in constraint vectors c1 and c2 does not match.");
        }
        
    }
    
    virtual vtkSmartPointer<vtkDoubleArray> transferFunction(vtkSmartPointer<vtkDoubleArray> f) = 0;
    
    virtual ~FunctionalMaps();

protected:
    Shape* shape1_;
    Shape* shape2_;
    vector<vtkSmartPointer<vtkDoubleArray>>& c1_;
    vector<vtkSmartPointer<vtkDoubleArray>>& c2_;
    int numberOfEigenfunctions_;
    int numberOfConstraints_;
};


#endif /* defined(__ShapeAnalyzer__FunctionalMaps__) */

