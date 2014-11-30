#ifndef __ShapeAnalyzer__FunctionalMaps__
#define __ShapeAnalyzer__FunctionalMaps__

#include "Shape.h"

#include <vtkDoubleArray.h>

#include <vector>

using namespace std;

/// \brief Abstract class for the transfer of a function from one shape to another using the functional maps approach.
class FunctionalMaps {
public:
    /// \brief Constructor
    /// @param Shape* Source shape.
    /// @param Shape* Target shape.
    /// @param vector<vtkSmartPointer<vtkDoubleArray>>& Vector of constraints (Vertex-to-Double maps) defined on the source shape.
    /// @param vector<vtkSmartPointer<vtkDoubleArray>>& Vector of constraints (Vertex-to-Double maps) defined on the target shape.
    /// @param int Number of eigenfunctions that are used for the representation of the functions.
    FunctionalMaps(Shape* shape1, Shape* shape2, vector<vtkSmartPointer<vtkDoubleArray>>& c1, vector<vtkSmartPointer<vtkDoubleArray>>& c2, int numberOfEigenfunctions) : shape1_(shape1), shape2_(shape2), c1_(c1), c2_(c2), numberOfEigenfunctions_(numberOfEigenfunctions) {
        numberOfConstraints_ = c1_.size();
        
        if(c1_.size() != c2_.size()) {
            throw new invalid_argument("Number of elements in constraint vectors c1 and c2 does not match.");
        }
        
    }
    
    /// \brief Transfers and returns a function f from the source to the target shape.
    virtual vtkSmartPointer<vtkDoubleArray> transferFunction(vtkSmartPointer<vtkDoubleArray> f) = 0;
    
    virtual ~FunctionalMaps() {}

protected:
    Shape* shape1_;
    Shape* shape2_;
    vector<vtkSmartPointer<vtkDoubleArray>>& c1_;
    vector<vtkSmartPointer<vtkDoubleArray>>& c2_;
    int numberOfEigenfunctions_;
    int numberOfConstraints_;
};


#endif /* defined(__ShapeAnalyzer__FunctionalMaps__) */

