//
//  LaplaceBeltramiSignature.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#ifndef __ShapeAnalyzer__LaplaceBeltramiSignature__
#define __ShapeAnalyzer__LaplaceBeltramiSignature__

#include "Signature.h"

using namespace laplaceBeltrami;

namespace signature {

class LaplaceBeltramiSignature : public Signature {
public:
    LaplaceBeltramiSignature(Shape *shape, int dimension, LaplaceBeltramiOperator* laplacian);
    
    virtual ~LaplaceBeltramiSignature();
    
    void getComponent(int i, Vec* component);
    virtual vtkSmartPointer<vtkDoubleArray> getComponent(int i);
protected:
    Mat signature_;
    LaplaceBeltramiOperator* laplacian_;
};
    
}

#endif /* defined(__ShapeAnalyzer__LaplaceBeltramiSignature__) */
