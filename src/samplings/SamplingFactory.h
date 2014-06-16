//
//  SamplingFactory.h
//  ShapeAnalyzer
//
//  Created by Zorah on 05.06.14.
//
//

#ifndef ShapeAnalyzer_SamplingFactory_h
#define ShapeAnalyzer_SamplingFactory_h

#include "ShapeSampling.h"

#include "FPS.h"

class SamplingFactory {
public:
    SamplingFactory() {};
    ~SamplingFactory() {};
    
    string getLabel() {
        return "Sampling";
    }
    
    vector<string> getIdentifier() {
        vector<string> ids = vector<string>();
        ids.push_back("FPS");
        
        return ids;
    }
    
    ShapeSampling* produce(string identifier, Shape* shape) {
        //if(identifier == "FPS")
        //    return new FPS(shape);
        
        return nullptr;
    }
    
};


#endif
