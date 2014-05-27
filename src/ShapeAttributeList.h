//
//  ShapeAttributeList.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.05.14.
//
//

#ifndef ShapeAnalyzer_ShapeAttributeList_h
#define ShapeAnalyzer_ShapeAttributeList_h

#include <vtkSmartPointer.h>

#include "ShapeAttribute.h"
#include "vtkShape.h"

using namespace std

class ShapeAttributeList {
    
public:
    String[] getMetricLabels() {
        return {
                    "Euclidean",
                    "Geodesic"
                };
    }
    
    ShapeAttribute* newAttribute(vtkSmartPointer<vtkShape>, String label) {
        if(label == "Euclidean") {
            return new vtkGeodesic();
        }
    }
    
};


#endif
