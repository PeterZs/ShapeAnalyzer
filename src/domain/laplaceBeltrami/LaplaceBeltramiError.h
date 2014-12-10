#ifndef ShapeAnalyzer_LaplaceBeltramiError_h
#define ShapeAnalyzer_LaplaceBeltramiError_h

#include "../util/Error.h"

/// \brief Specific class for errors occurring with the metric calculation.
/// @author Emanuel Laude and Zorah Lähner
class LaplaceBeltramiError : public Error {
public:
    LaplaceBeltramiError() : Error() {}
    LaplaceBeltramiError(const std::string& str) : Error(str) {}
    ~LaplaceBeltramiError() {}
};


#endif
