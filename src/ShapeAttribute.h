//
//  ShapeAttribute.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.05.14.
//
//

#ifndef ShapeAnalyzer_ShapeAttribute_h
#define ShapeAnalyzer_ShapeAttribute_h

class ShapeAttribute {
    
public:
    virtual ShapeAttribute(Shape *shape) = 0;
    
private:
    Shape *shape_;
};


#endif
