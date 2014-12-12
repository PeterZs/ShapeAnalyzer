#ifndef __ShapeAnalyzer__PetscFunctionalMaps__
#define __ShapeAnalyzer__PetscFunctionalMaps__

#include "FunctionalMaps.h"

#include "laplaceBeltrami/PetscLaplaceBeltramiOperator.h"

#include "PetscHelper.h"

#include <slepceps.h>

using namespace laplaceBeltrami;

/// \brief Concrete implementation of the FunctionalMaps class using the PETSC framework.
class PetscFunctionalMaps : public FunctionalMaps {
public:
    /// \brief Constructor
    /// @param Shape* Source shape.
    /// @param Shape* Target shape.
    /// @param PetscLaplaceBeltramiOperator* Laplacian of source shape.
    /// @param PetscLaplaceBeltramiOperator* Laplacian of target shape.
    /// @param vector<vtkSmartPointer<vtkDoubleArray>>& Vector of constraints (Vertex-to-Double maps) defined on the source shape.
    /// @param vector<vtkSmartPointer<vtkDoubleArray>>& Vector of constraints (Vertex-to-Double maps) defined on the target shape.
    /// @param int Number of eigenfunctions that are used for the representation of the functions.
    PetscFunctionalMaps(shared_ptr<Shape> shape1, shared_ptr<Shape> shape2, shared_ptr<PetscLaplaceBeltramiOperator> laplacian1, shared_ptr<PetscLaplaceBeltramiOperator> laplacian2, vector<vtkSmartPointer<vtkDoubleArray>>& c1, vector<vtkSmartPointer<vtkDoubleArray>>& c2, int numberOfEigenfunctions);
    
    vtkSmartPointer<vtkDoubleArray> transferFunction(vtkSmartPointer<vtkDataArray> f);
    
    /// \brief Virtual destructor.
    ~PetscFunctionalMaps();
private:
    
    /// \brief Computes the matrix Phi^T * M that is used for further computations where Phi is the matrix containing the eigenfunctions as columns and M is the Mass matrix of the respective shape.
    void setupPhiTM(Shape* shape, PetscLaplaceBeltramiOperator* laplacian, Mat* Phi, Mat *PhiTM);
    
    /// \brief Correspondence in Functional Maps representation
    Mat C_;
    
    Mat AT_;
    Vec b_;
    
    /// \brief Eigenfunctions of source shape stacked as columns into a matrix
    Mat Phi1_;
    
    /// \brief Eigenfunctions of target shape stacked as columns into a matrix
    Mat Phi2_;
    
    /// \brief the matrix {Phi_1}^T * M_1
    Mat PhiTM1_;
    
    /// \brief the matrix {Phi_2}^T * M_2
    Mat PhiTM2_;
    
    /// \brief Laplacian of source shape.
    shared_ptr<PetscLaplaceBeltramiOperator> laplacian1_;
    
    /// \brief Laplacian of source shape.
    shared_ptr<PetscLaplaceBeltramiOperator> laplacian2_;

    /// \brief PETSC least squares solver
    KSP ksp_;
};


#endif /* defined(__ShapeAnalyzer__FunctionalMaps__) */

