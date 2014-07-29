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
    
    virtual ostream& write(ostream& os);
    
    virtual istream& read(istream& is);
    
};

#endif
