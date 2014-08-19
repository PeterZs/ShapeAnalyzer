//
//  PointCorrespondeceData.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 08.05.14.
//
//

#ifndef PointCorrespondeceData_H
#define PointCorrespondeceData_H

#include "CorrespondenceData.h"

//serves as a marker class to ensure that variable "HashMap<...> pointCorrespondences" only contains point correspondences

class PointCorrespondenceData : public CorrespondenceData {
public:
    PointCorrespondenceData() {
    }
    
};

#endif /* defined(PointCorrespondeceData_H) */
