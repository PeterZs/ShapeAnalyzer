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
                               HashMap<PointCorrespondenceData*, bool>* pointCorr,
                               HashMap<vtkActor*, Shape*>*              shapes,
                               string                               filename
                               );
    static void exportFaceCorrespondences(
                               HashMap<FaceCorrespondenceData*, bool>*  faceCorr,
                               HashMap<vtkActor*, Shape*>*              shapes,
                               string                               filename
                               );
    static void importCorrespondences(
                                    string                                  filename,
                                    HashMap<PointCorrespondenceData*, bool>*    pointCorr,
                                    HashMap<FaceCorrespondenceData*, bool>*     faceCorr,
                                    QListWidget*                            shapes,
                                    QWidget*                                parentWidget
                                    );
    
private:
    static void exportCorrespondences(
                               HashMap<CorrespondenceData*, bool>*  pointCorr,
                               HashMap<vtkActor*, Shape*>*          shapeIds,
                               ofstream*                        os
                               );
    static unordered_map<string, Shape*> createShapeMap(QListWidget* shapes);
    static void createData(vector<Shape*> shapes, stringstream* ss, CorrespondenceData* data);
};

#endif /* defined(__ShapeAnalyzer__SceneWriterReader__) */
