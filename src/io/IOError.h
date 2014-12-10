#ifndef ShapeAnalyzer_IOError_h
#define ShapeAnalyzer_IOError_h

#include "../util/Error.h"

class IOError : public Error {
    IOError() : Error() {}
    IOError(const std::string& str) : Error(str) {}
    ~IOError() {}
};

#endif
