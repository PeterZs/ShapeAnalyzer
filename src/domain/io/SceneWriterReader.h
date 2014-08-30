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
    
    // whole scenes
    static void importSceneBinary(string filename, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, vector<Shape*>& shapes, int& lastInsertCorrespondenceID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences);
    static void exportSceneBinary(string filename, vector<Shape*>& shapes, int lastInsertShapeID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences, int lastInsertCorrespondenceID);
    static void importSceneASCII(string filename, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, vector<Shape*>& shapes, int& lastInsertCorrespondenceID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences);
    static void exportSceneASCII(string filename, vector<Shape*>& shapes, int lastInsertShapeID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences, int lastInsertCorrespondenceID);
    
    // correspondences
    static void exportPointCorrespondencesASCII(
                               HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                               vector<Shape*>&                          shapesOrderedById,
                               string                                   filename
                               );
    static void exportFaceCorrespondencesASCII(
                               HashMap<FaceCorrespondenceData*, bool>&  faceCorrespondences,
                               vector<Shape*>&                          shapesOrderedById,
                               string                                   filename
                               );
    
    static void importCorrespondencesASCII(string                               filename,
                                      int&                                      lastInsertCorrespondenceID_,
                                      vector<PointCorrespondenceData*>&         pointCorrespondences,
                                      vector<FaceCorrespondenceData*>&          faceCorrespondences,
                                      vector<Shape*>&                           shapesOrderedById,
                                      QWidget*                                  parentWidget);

    
    static void exportPointCorrespondencesBinary(
                                                HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                                                vector<Shape*>&                          shapesOrderedById,
                                                string                                   filename
                                                );
    static void exportFaceCorrespondencesBinary(
                                               HashMap<FaceCorrespondenceData*, bool>&  faceCorrespondences,
                                               vector<Shape*>&                          shapesOrderedById,
                                               string                                   filename
                                               );
    
    static void importCorrespondencesBinary(string                                   filename,
                                           int&                                      lastInsertCorrespondenceID_,
                                           vector<PointCorrespondenceData*>&         pointCorrespondences,
                                           vector<FaceCorrespondenceData*>&          faceCorrespondences,
                                           vector<Shape*>&                           shapesOrderedById,
                                           QWidget*                                  parentWidget);
private:
    static void exportCorrespondencesASCII(HashMap<CorrespondenceData*, bool>&  correspondences,
                                      vector<Shape*>&                           shapesOrderedById,
                                      ostream&                                  os);

    static void createCorrespondenceDataASCII(vector<Shape*>& shapes, stringstream& ss, CorrespondenceData* data);
    
    static void exportCorrespondencesBinary(HashMap<CorrespondenceData*, bool>&  correspondences,
                                           vector<Shape*>&                           shapesOrderedById,
                                           ostream&                                  os);

};

#endif /* defined(__ShapeAnalyzer__SceneWriterReader__) */
