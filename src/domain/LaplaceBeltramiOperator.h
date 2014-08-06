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

#include <slepceps.h>

class LaplaceBeltramiOperator {
public:
    LaplaceBeltramiOperator(vtkSmartPointer<vtkPolyData> polyData, int numberOfEigenfunctions);
    
    virtual ~LaplaceBeltramiOperator() {};
    
    virtual void initialize() = 0;

    virtual double getEigenvalue(vtkIdType i) = 0;
    
    virtual void getEigenfunction(PetscInt i, PetscScalar** phi) = 0;
    
    virtual void getEigenfunction(PetscInt i, Vec* phi) = 0;
    
    virtual void getEigenpair(PetscInt i, Vec* phi, PetscScalar* lambda) = 0;
    
    virtual void getEigenpair(PetscInt i, PetscScalar** phi, PetscScalar* lambda) = 0;
    
    virtual void getMassMatrix(Mat* M) = 0;
    
    int getNumberOfEigenfunctions() {
        return numberOfEigenfunctions_;
    }
protected:
    vtkSmartPointer<vtkPolyData> polyData_;
    int numberOfEigenfunctions_;
};

#endif
