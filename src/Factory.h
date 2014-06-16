//
//  Factory.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_Factory_h
#define ShapeAnalyzer_Factory_h

using namespace std;

template <class T> class Factory {
public:
    virtual string getLabel() = 0;
    virtual vector<string> getIdentifier() = 0;
    virtual T* produce(string identifier, Shape* shape);
};


#endif
