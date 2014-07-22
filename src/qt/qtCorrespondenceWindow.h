//
//  qtCorrespondenceWindow.h
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#ifndef __ShapeAnalyzer__qtCorrespondenceWindow__
#define __ShapeAnalyzer__qtCorrespondenceWindow__

#include <QDialog>

#include "../CorrespondenceSet.h"
#include "../FaceCorrespondenceData.h"
#include "../PointCorrespondenceData.h"

#include "ui_correspondences.h"

using namespace std;

class qtCorrespondenceWindow {
public:
    qtCorrespondenceWindow(
                           CorrespondenceSet<PointCorrespondenceData>* points,
                           CorrespondenceSet<FaceCorrespondenceData>*  faces
                           );
    ~qtCorrespondenceWindow() {}
    
private:
    CorrespondenceSet<PointCorrespondenceData>* pointCorr_;
    CorrespondenceSet<FaceCorrespondenceData>*  faceCorr_;
    
    Ui_CorrespondenceWindow window_;
};


#endif /* defined(__ShapeAnalyzer__qtCorrespondenceWindow__) */
