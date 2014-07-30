//
//  ShapeAttribute.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_ShapeAttribute_h
#define ShapeAnalyzer_ShapeAttribute_h

template<T>
class ShapeAttribute {
public:
    virtual ~ShapeAttribute() {};
    
    virtual T getValue() = 0;
};


#endif
