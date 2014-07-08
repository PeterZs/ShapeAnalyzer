//
//  CorrespondenceAttribute.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_CorrespondenceAttribute_h
#define ShapeAnalyzer_CorrespondenceAttribute_h

#include "../Correspondence.h"

class CorrespondenceAttribute {
    
public:
    ~CorrespondenceAttribute() {};
    
    virtual double[] getValues() = 0;
    
protected:
    
    
};


#endif
