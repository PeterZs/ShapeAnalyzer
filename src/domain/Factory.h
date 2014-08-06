//
//  Factory.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 06.08.14.
//
//

#ifndef __ShapeAnalyzer__Factory__
#define __ShapeAnalyzer__Factory__

#include <string>
#include <unordered_map>

#include "Shape.h"

using namespace std;

//Factory is a singleton
template<class T>
class Factory {
public:
    typedef T* (CreateFn)(void);
    
    
    ~Factory() {
        createFns_.clear();
    }
    
    //only way to obtain the unique Factory<T> object
    static Factory<T>* getInstance() {
        static Factory<T> instance;
        return &instance;
    }
    
    //register new T with unique identifier, label (for GUI purposes) and "create-function-pointer"
    void Register(const string& identifier, const string& label, CreateFn* fn);
    
    //create a new instance of the desired object
    T* create(const string& identifier);
    
    unordered_map<string, string>& getLabels() {
        return labels_;
    }
private:
    Factory() {
        
    }
    
    Factory(const Factory&) {
    }
    
    Factory& operator=(const Factory&) {
        return *this;
    }
    
    unordered_map<string, CreateFn*> createFns_;
    unordered_map<string, string> labels_;
};

template<class T>
void Factory<T>::Register(const string& identifier, const string& label, CreateFn* fn) {
    
    createFns_.insert(pair<string, CreateFn*>(identifier, fn));
    labels_.insert(pair<string, string>(identifier, label));
}

template<class T>
T* Factory<T>::create(const string& identifier) {
    
    if(createFns_.find(identifier) != createFns_.end()) {
        return createFns_.find(identifier)->second();
    }
    return nullptr;
}

#endif /* defined(__ShapeAnalyzer__Factory__) */
