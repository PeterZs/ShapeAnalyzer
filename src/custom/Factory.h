#ifndef __ShapeAnalyzer__Factory__
#define __ShapeAnalyzer__Factory__

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

///
/// \brief Variadic template factory class for creating instances of concrete CustomTab, CustomContextMenuItem and other classes.
///
/// \details Concrete classes are derived from an abstract superclass for example ColorEigenfunctionContextMenuItem is derived from the abstract class ContextMenuItem. Moreover concrete classes
/// have to provide a create function with an arbitrary std::tuple of arguments that returns a new instance of this class. The tuple can also be empty.
/// @tparam class T. Abstract class for which the factory should be defined (for example Metric or LaplaceBeltramiSignature)
/// @tparam typename... Args. Argument signature of the create functions of type T.
///
/// \author Emanuel Laude and Zorah Lähner
///
template<class T, class... Args>
class Factory {
public:
    typedef T (*CreateFunction)(Args...);
    
    /// \brief Destructor. Clears the internal data structures.
    ~Factory() {
        createFunctions_.clear();
    }
    
    /// \brief Returns the unique instance of Factory<T>.
    /// \details This is the only way to obtain the Factory object for type T.
    static Factory<T, Args...>* getInstance() {
        static Factory<T, Args...> instance;
        return &instance;
    }

    /// \brief Registers concrete class derived from type T in the internal data structures of Factory.
    /// @tparam class C. The concrete class to be registered.
    /// @param const string& identifier. A unique string that is used as a key to obtain an instance via create() afterwards.
    /// @param const string& label. A label that can for example serve as menu path in case of CustomContextMenuItem
    template<class C>
    void Register(const string& identifier, const string& label){
        createFunctions_.insert(pair<string, CreateFunction>(identifier, &C::create));
        labels_.push_back(pair<string, string>(identifier, label));
        labelIndex_.insert(pair<string, int>(identifier, labels_.size() - 1));
    }
    
    /// \brief Obtain a new instance of a class derived from T using the unique identifier of the class and a std::tuple of arguments required to instantiate the class. Throws an exception of type std::out_of_range if identifier is not present.
    /// @param const string& identifier. identifier of the class.
    /// @param std::tuple of arguments for the create function.
    T create(const string& identifier, Args... args) {
        // execute create function pointer given the arguments in tuple args.
        return createFunctions_.at(identifier)(args...);
    }
    
    /// \brief Returns a map contianing all identifier plus label pairs that have been registered in this particular factory.
    const vector<pair<string, string>>& getLabels() const {
        return labels_;
    }
    
    /// \brief Returns the label for a given identifier. Throws an exception of type std::out_of_range if identifier is not present.
    /// @param const string& identifier.
    const string& getLabel(const string& identifier) const {
        return labels_.at(labelIndex_.at(identifier)).second;
    }
private:
    /// \brief Private constructor.
    /// \details Use getInstance() to obtain the unique instance of this singleton.
    Factory() {
    }
    
    /// \brief Private copy constructor.
    /// \details Use getInstance() to obtain the unique instance of this singleton.
    Factory(const Factory&) {
    }
    
    /// \brief Private assignment operator.
    /// \details Use getInstance() to obtain the unique instance of this singleton.
    Factory& operator=(const Factory&) {
        return *this;
    }
    
    /// \brief Map containing all identifier plus create function pointer pairs.
    unordered_map<string, CreateFunction> createFunctions_;
    
    /// \brief Vector containing all identifier plus label pairs.
    /// \details Vector preserves insertion ordering.
    vector<pair<string, string>> labels_;
    
    /// \brief Map for fast retrieval of label given a specific identifier.
    /// \details Vector preserves insertion ordering.
    unordered_map<string, int> labelIndex_;
};

#endif /* defined(__ShapeAnalyzer__Factory__) */
