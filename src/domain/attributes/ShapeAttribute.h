//
//  ShapeAttribute.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_ShapeAttribute_h
#define ShapeAnalyzer_ShapeAttribute_h

template<class T>
class ShapeAttribute {
public:
    ~ShapeAttribute() {};
    
    T* getValue();
private:
    T* value_;
    
};


#endif
