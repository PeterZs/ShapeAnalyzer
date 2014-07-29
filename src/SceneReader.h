//
//  SceneReader.h
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#ifndef __ShapeAnalyzer__SceneReader__
#define __ShapeAnalyzer__SceneReader__

#include "FaceCorrespondence.h"
#include "PointCorrespondence.h"
#include "Set.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class SceneReader {
    
public:
    SceneReader() {};
    ~SceneReader() {};
    
    static void exportPointCorrespondences(
                               Set<PointCorrespondenceData*, bool>* pointCorr,
                               Set<vtkActor*, Shape*>*              shapes,
                               string                               filename
                               );
    static void exportFaceCorrespondences(
                               Set<FaceCorrespondenceData*, bool>*  pointCorr,
                               Set<vtkActor*, Shape*>*              shapes,
                               string                               filename
                               );
    
private:
    static void exportCorrespondences(
                               Set<CorrespondenceData*, bool>*  pointCorr,
                               Set<vtkActor*, Shape*>*          shapeIds,
                               ofstream*                        os
                               );
};

#endif /* defined(__ShapeAnalyzer__SceneReader__) */
