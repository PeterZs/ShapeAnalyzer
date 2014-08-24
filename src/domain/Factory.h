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


using namespace std;

//Factory is a singleton
template<class T>
class Factory {
public:
    typedef T* (*CreateFn)(void);
    
    
    ~Factory() {
        createFns_.clear();
    }
    
    // returs unique instance of Factory<T>. Only way to obtain the Factory object for type T.
    static Factory<T>* getInstance() {
        static Factory<T> instance;
        return &instance;
    }
    
    //register a new class. Template argument class name 'C'. Argument label (for GUI purposes). Create-function pointer and identifier have to be provided via the static functions C::getIdentifier and C::create of the class 'C' itself.
    template<class C>
    void Register(const string& label){
        
        createFns_.insert(pair<string, CreateFn>(C::getIdentifier(), &C::create));
        labels_.insert(pair<string, string>(C::getIdentifier(), label));
    }
    
    //create a new instance of the desired object using the identifier
    T* create(const string& identifier) {
        typename std::unordered_map<string, CreateFn>::iterator it = createFns_.find(identifier);
        if(it != createFns_.end()) {
            return createFns_.find(identifier)->second();
        }
        return nullptr;
    }
    
    //create a new instance of the desired object using just the Class name provided as template argument 'C'.
    template<class C>
    T* create() {
        typename std::unordered_map<string, CreateFn>::iterator it = createFns_.find(C::getIdentifier());
        if(it != createFns_.end()) {
            return it->second();
        }
        return nullptr;
    }
    
    
    // returns map of all labels that have been registered with this particular factory. Labels indexed by the identifier provided by each concrete class via static function C::getIdentifier
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
    
    unordered_map<string, CreateFn> createFns_; // map of all create function pointers. Keys: identifiers
    unordered_map<string, string> labels_; // map of all labels. Keys: identifiers
};

#endif /* defined(__ShapeAnalyzer__Factory__) */
