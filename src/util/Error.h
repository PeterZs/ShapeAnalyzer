#ifndef ShapeAnalyzer_Exception_h
#define ShapeAnalyzer_Exception_h

using namespace std;

///
/// \brief Used to throw exceptions within the ShapeAnalyzer.
/// @author Emanuel Laude and Zorah Lähner
///
class Error : public exception {
public:
    /// \brief Standard constructor.
    /// \details If possible use the specific constructor Error(const std::string&) and
    /// give a message indicating the kind of problem.
    Error() : what_("An error occured in the Shape Analyzer framework.") {}
    /// \brief Specific constructor.
    /// \details The given string will be shown in an error dialog. Please make it meaningful.
    Error(const std::string& str) : what_(str) {
    }
    
    /// \brief Returns the what_-message indicating the kind of problem.
    virtual const char* what() const throw() { return what_.c_str(); }
    
private:
    std::string what_;
}

#endif
