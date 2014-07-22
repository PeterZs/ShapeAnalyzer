//
//  geodesic_error.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#ifndef ShapeAnalyzer_geodesic_error_h
#define ShapeAnalyzer_geodesic_error_h

#include <string>
#include <exception>

class geodesic_error {
public:
    geodesic_error() : what_("An error occured while calculating the geodesic.") {}
    geodesic_error(std::string str) : what_(str) {}
    
    std::string what() { return what_; }
    
private:
    std::string what_;
};


#endif
