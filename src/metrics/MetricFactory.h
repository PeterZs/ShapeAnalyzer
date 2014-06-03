//
//  MetricFactory.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#ifndef ShapeAnalyzer_MetricFactory_h
#define ShapeAnalyzer_MetricFactory_h

#include "Euclidean.h"
#include "Geodesic.h"

#include <Shape.h>

using namespace std;

class MetricFactory {
    
public:
    MetricFactory() {};
    ~MetricFactory() {};
    
    vector<string> getIdentifier() {
        vector<string> ids = vector<string>();
        ids.push_back("Euclidean");
        ids.push_back("Geodesic");
        
        return ids;
    }
    
    Metric* produce(string identifier, Shape* shape) {
        if(identifier == "Euclidean")
            return new Euclidean(shape);
        
        if(identifier == "Geodesic")
            return new Geodesic(shape);
        
        return nullptr;
    }
};


#endif
