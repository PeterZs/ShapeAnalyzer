#ifndef __ShapeAnalyzer__Factory__
#define __ShapeAnalyzer__Factory__

#include <string>
#include <unordered_map>
#include <memory>
#include <tuple>

using namespace std;

///
/// \brief Templated factory class for creating instances of concrete colorings, segmentations, signatures and so on.
///
/// \details Concrete classes are derived from an abstract superclass for example EuclideanMetric is derived from abstract class Metric. Moreover concrete classes
/// have to provide both a create function that returns a new instance of this class and a getIdentifier function that returs a unique string that is used
/// to register the concrete class in the factory.
/// @tparam class T. Abstract class for which the factory should be defined (for example Metric or LaplaceBeltramiSignature)
///
/// \author Emanuel Laude and Zorah Lähner
///
template<class T, typename... Args>
class Factory {
public:
    typedef shared_ptr<T> (*CreateFn)(Args...);
    
    /// Destructor.
    ~Factory() {
        createFns_.clear();
    }
    
    /// \brief Returns the unique instance of Factory<T>. This is the only way to obtain the Factory object for type T.
    static Factory<T>* getInstance() {
        static Factory<T> instance;
        return &instance;
    }

    template<class C>
    void Register(const string& identifier, const string& label){
        createFns_.insert(pair<string, CreateFn>(identifier, &C::create));
        labels_.insert(pair<string, string>(identifier, label));
    }
    
    //create a new instance of the desired object using the identifier
    shared_ptr<T> create(const string& identifier, Args... args) {
        typename std::unordered_map<string, CreateFn>::iterator it = createFns_.find(identifier);
        if(it != createFns_.end()) {
            return createFns_.find(identifier)->second(args...);
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
