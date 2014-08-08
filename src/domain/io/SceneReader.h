//
//  SceneReader.h
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#ifndef __ShapeAnalyzer__SceneReader__
#define __ShapeAnalyzer__SceneReader__

#include "../correspondences/CorrespondenceData.h"
#include "../correspondences/FaceCorrespondence.h"
#include "../correspondences/FaceCorrespondenceData.h"
#include "../correspondences/PointCorrespondence.h"
#include "../correspondences/PointCorrespondenceData.h"
#include "../Set.h"

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

class SceneReader {
    
public:
    SceneReader() {};
    ~SceneReader() {};
    
    // correspondences
    static void exportPointCorrespondences(
                               Set<PointCorrespondenceData*, bool>* pointCorr,
                               Set<vtkActor*, Shape*>*              shapes,
                               string                               filename
                               );
    static void exportFaceCorrespondences(
                               Set<FaceCorrespondenceData*, bool>*  faceCorr,
                               Set<vtkActor*, Shape*>*              shapes,
                               string                               filename
                               );
    static void loadCorrespondences(
                                    string                                  filename,
                                    Set<PointCorrespondenceData*, bool>*    pointCorr,
                                    Set<FaceCorrespondenceData*, bool>*     faceCorr,
                                    QListWidget*                            shapes,
                                    QWidget*                                parentWidget
                                    );
    
private:
    static void exportCorrespondences(
                               Set<CorrespondenceData*, bool>*  pointCorr,
                               Set<vtkActor*, Shape*>*          shapeIds,
                               ofstream*                        os
                               );
    static unordered_map<string, Shape*> createShapeMap(QListWidget* shapes);
    static void createData(vector<Shape*> shapes, stringstream* ss, CorrespondenceData* data);
};

#endif /* defined(__ShapeAnalyzer__SceneReader__) */
