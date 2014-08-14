//
//  WaveKernelSignature.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 13.08.14.
//
//

#include "WaveKernelSignature.h"

void WaveKernelSignature::initialize(Shape* shape, int dimension) {
    PointSignature::initialize(shape, dimension);

    if(numberOfEigenfunctions_ == -1)
        numberOfEigenfunctions_ = std::min((int) shape_->getPolyData()->GetNumberOfPoints(), 100);
    

    
    
    PetscScalar* logLambda = new PetscScalar[numberOfEigenfunctions_];
    
    LaplaceBeltramiOperator* laplacian = shape_->getLaplacian(numberOfEigenfunctions_);
    
//    
//    ofstream out;
//    out.open("Phi.csv");
//    
//    for(int i = 0; i < 100; i++) {
//        Vec phi;
//        
//        laplacian->getEigenfunction(i, &phi);
//        
//        for(int j = 0; j < shape_->getPolyData()->GetNumberOfPoints(); j++) {
//            PetscScalar val;
//            
//            VecGetValues(phi, 1, &j, &val);
//            out << val <<(j < shape_->getPolyData()->GetNumberOfPoints()-1 ?  "," : "");
//        }
//        out <<endl;
//        
//        VecDestroy(&phi);
//    }
//    
//    out.close();
//
//    ofstream out2;
//    
//    out2.open("lambda.csv");
//    
//    for(int i = 0; i < 100; i++) {
//
//        
//        out2 <<laplacian->getEigenvalue(i);
//        out2 <<endl;
//        
//    }
//    
//    out2.close();
//
//    ofstream out3;
//    
//    out3.open("shape.off");
//    
//    out3 << "OFF"<<endl;
//    out3 << shape_->getPolyData()->GetNumberOfPoints() <<" "<<shape_->getPolyData()->GetNumberOfCells()<<" 0"<<endl;
//    
//    for(vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfPoints(); i++) {
//        double p[3];
//        shape_->getPolyData()->GetPoint(i, p);
//        out3 << p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
//    }
//
//    for(vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfCells(); i++) {
//        vtkTriangle* t = (vtkTriangle*) shape_->getPolyData()->GetCell(i);
//        
//        out3 <<"3 "<<t->GetPointId(0)<<" "<<t->GetPointId(1)<<" "<<t->GetPointId(2)<<endl;
//    }
//    
//    out3.close();
    
    double maximum = -std::numeric_limits<double>::infinity();
    for(PetscInt i = 0; i < numberOfEigenfunctions_; i++) {
        PetscScalar lambda = laplacian->getEigenvalue(i);
        logLambda[i]= log(std::max((abs(lambda)), 1e-6));
        if(maximum < logLambda[i]) {
            maximum = logLambda[i];
        }
    }
    
    
    PetscScalar step = (maximum / 1.02 - logLambda[1]) / (dimension_ - 1);
    PetscScalar* e = new PetscScalar[dimension_];

    for(PetscInt i = 0; i < dimension_; i++) {
        e[i] = logLambda[1] + i*step;
    }
    
    PetscScalar sigma = (e[1] - e[0]) * wksVariance_;
    
    

    for(PetscInt i = 0; i < dimension_; i++) {
        Vec wksi; //i-th component of wks
        VecCreateSeq(MPI_COMM_SELF, shape_->getPolyData()->GetNumberOfPoints(), &wksi);
        VecSet(wksi, 0.0);
        PetscScalar C = 0;
        for(PetscInt k = 0; k < numberOfEigenfunctions_; k++) {
            Vec phi;
            laplacian->getEigenfunction(k, &phi);
            VecPow(phi, 2.0);
            
            PetscScalar c = exp( -pow(e[i] - logLambda[k], 2.0) / ( 2.0 * sigma * sigma ));
            VecAXPY(wksi, c, phi);
            C += c;
            
            
            VecDestroy(&phi);
        }
        
        VecScale(wksi, C);
        
        
        PetscHelper::setRow(signature_, wksi, i);
        
        VecDestroy(&wksi);
    }
    MatAssemblyBegin(signature_, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(signature_, MAT_FINAL_ASSEMBLY);
    
    delete [] logLambda;
    delete [] e;
    
}