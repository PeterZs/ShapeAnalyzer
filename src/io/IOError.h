#ifndef ShapeAnalyzer_IOError_h
#define ShapeAnalyzer_IOError_h

#include "../util/Error.h"

namespace io {

/// \brief Special Error for exceptions occuring while writing or reading scenes/shapes/correspondenes.
class IOError : public Error {
public:
    IOError() : Error() {}
    IOError(const std::string& str) : Error(str) {}
    ~IOError() {}
};

}

#endif
