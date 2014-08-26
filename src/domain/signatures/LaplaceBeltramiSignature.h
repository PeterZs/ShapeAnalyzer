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

class LaplaceBeltramiSignature : public Signature {
public:
    virtual ~LaplaceBeltramiSignature();
    
    virtual void initialize(Shape* shape, int dimension);
    
    void setLaplacian(LaplaceBeltramiOperator* laplacian) {
        laplacian_ = laplacian;
    }
    
    
    void getComponent(int i, Vec* component);
    virtual void getComponent(int i, ScalarPointAttribute& component);
protected:
    LaplaceBeltramiSignature() {
    }

    Mat signature_;
    LaplaceBeltramiOperator* laplacian_;
};

#endif /* defined(__ShapeAnalyzer__LaplaceBeltramiSignature__) */
