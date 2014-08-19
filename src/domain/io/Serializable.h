//
//  Serializable.h
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#ifndef ShapeAnalyzer_Serializable_h
#define ShapeAnalyzer_Serializable_h

#include <iostream>
#include <string>
#include <sstream>

class Serializable {
    
    virtual ostream& writeBinary(ostream& os) = 0;
    
    virtual ostream& writeASCII(ostream& os) = 0;
    
    virtual istream& readBinary(istream& is) = 0;
    
    virtual istream& readASCII(istream& is) = 0;
    
};

#endif
