//
//  qtCorrespondenceWindow.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 21.07.14.
//
//

#include "qtCorrespondenceWindow.h"

qtCorrespondenceWindow::qtCorrespondenceWindow(CorrespondenceSet<PointCorrespondenceData>* points,
                                               CorrespondenceSet<FaceCorrespondenceData>* faces) {
    pointCorr_ = points;
    faceCorr_ = faces;
    
    QDialog* correspondences = new QDialog(0,0);

    window_.setupUi(correspondences);
    
    // add all correspondences to widget
    //for(typename unordered_map<PointCorrespondenceData*, bool>::iterator it = pointCorr_->begin(); it != pointCorr_->end(); it++) {
        
    //}
    
    correspondences->show();
}