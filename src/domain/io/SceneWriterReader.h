//
//  SceneWriterReader.h
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#ifndef __ShapeAnalyzer__SceneWriterReader__
#define __ShapeAnalyzer__SceneWriterReader__

#include "../correspondences/CorrespondenceData.h"
#include "../correspondences/FaceCorrespondence.h"
#include "../correspondences/FaceCorrespondenceData.h"
#include "../correspondences/PointCorrespondence.h"
#include "../correspondences/PointCorrespondenceData.h"
#include "../HashMap.h"

#include "../../view/qt/qtListWidgetItem.h"

#include <QInputDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QObject>
#include <QString>

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

class SceneWriterReader {
    
public:
    SceneWriterReader() {};
    ~SceneWriterReader() {};
    
    // correspondences
    static void exportPointCorrespondences(
                               HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                               vector<Shape*>&                          shapesOrderedById,
                               string                                   filename
                               );
    static void exportFaceCorrespondences(
                               HashMap<FaceCorrespondenceData*, bool>&  faceCorrespondences,
                               vector<Shape*>&                          shapesOrderedById,
                               string                                   filename
                               );
    
    static void importCorrespondences(string                            filename,
                                      int&                                      lastInsertCorrespondenceID_,
                                      HashMap<PointCorrespondenceData*, bool>&  pointCorrespondences,
                                      HashMap<FaceCorrespondenceData*, bool>&   faceCorrespondences,
                                      vector<Shape*>&                           shapesOrderedById,
                                      QWidget*                                  parentWidget);
    
private:
    static void exportCorrespondences(HashMap<CorrespondenceData*, bool>&       correspondences,
                                      vector<Shape*>&                           shapesOrderedById,
                                      ostream&                                  os);

    static void createCorrespondenceData(vector<Shape*>& shapes, stringstream& ss, CorrespondenceData* data);
};

#endif /* defined(__ShapeAnalyzer__SceneWriterReader__) */
