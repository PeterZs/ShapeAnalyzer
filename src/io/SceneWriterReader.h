#ifndef __ShapeAnalyzer__SceneWriterReader__
#define __ShapeAnalyzer__SceneWriterReader__

#include "../domain/correspondences/CorrespondenceData.h"
#include "../domain/correspondences/FaceCorrespondence.h"
#include "../domain/correspondences/FaceCorrespondenceData.h"
#include "../domain/correspondences/PointCorrespondence.h"
#include "../domain/correspondences/PointCorrespondenceData.h"
#include "../util/HashMap.h"

#include "../view/CustomListWidgetItem.h"

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

///
/// \brief Class for reading and writing whole scenes or lists of correspondences in Binary as well as in ASCII.
/// \details Provides functionality for both reading/writing of whole scenes including point and face correspondences and reading/writing of face or point correspondences separately.
/// \author Emanuel Laude and Zorah Lähner
///
class SceneWriterReader {
    
public:
    SceneWriterReader() {};
    ~SceneWriterReader() {};
    
   /// \brief Reads a scene including both shapes and correspondences from a binary file. It returns a vector of shapes two maps containing point and face correspondences respectively and the last used IDs for shapes and correspondences.
    static void importSceneBinary(string filename, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, vector<Shape*>& shapes, int& lastInsertCorrespondenceID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences);
   /// Exports a scene in binary including both shapes and correspondences. It takes an ordered vector of the shapes, two maps containing point face correspondences respectively and the last used IDs for shapes and correspondences as arguments.
    static void exportSceneBinary(string filename, vector<Shape*>& shapes, int lastInsertShapeID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences, int lastInsertCorrespondenceID);

    /// Reads a scene including both shapes and correspondences from an ASCII file. It returns a vector of shapes two maps containing point and face correspondences respectively and the last used IDs for shapes and correspondences.
    static void importSceneASCII(string filename, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, vector<Shape*>& shapes, int& lastInsertCorrespondenceID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences);
    
    /// Exports a scene in ASCII including both shapes and correspondences. It takes an ordered vector of the shapes, two maps containing point face correspondences respectively and the last used IDs for shapes and correspondences as arguments.
    static void exportSceneASCII(string filename, vector<Shape*>& shapes, int lastInsertShapeID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences, int lastInsertCorrespondenceID);
    
    /// Exports a list of point correspondences in ASCII. It takes a map containing PointCorrespondenceData objects a vector of the corresponding shapes ordered by ID and the filename as arguments.
    static void exportPointCorrespondencesASCII(
                               HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                               vector<Shape*>&                          shapesOrderedById,
                               string                                   filename
                               );
    
    /// Exports a list of face correspondences in ASCII. It takes a map containing PointCorrespondenceData objects a vector containing the corresponding shapes ordered by ID and the filename as arguments.
    static void exportFaceCorrespondencesASCII(
                               HashMap<FaceCorrespondenceData*, bool>&  faceCorrespondences,
                               vector<Shape*>&                          shapesOrderedById,
                               string                                   filename
                               );
    
    
    /// Reads two vectors of point and face correspondences respectively from an ASCII file.
    static void importCorrespondencesASCII(string                               filename,
                                      int&                                      lastInsertCorrespondenceID_,
                                      vector<CorrespondenceData*>&         pointCorrespondences,
                                      vector<CorrespondenceData*>&          faceCorrespondences,
                                      vector<Shape*>&                           shapesOrderedById,
                                      QWidget*                                  parentWidget);

    
    /// Exports a list of point correspondences in binary. It takes a map containing PointCorrespondenceData objects a vector containing the corresponding shapes ordered by ID and the filename as arguments.
    static void exportPointCorrespondencesBinary(
                                                HashMap<PointCorrespondenceData*, bool>& pointCorrespondences,
                                                vector<Shape*>&                          shapesOrderedById,
                                                string                                   filename
                                                );
    /// Exports a list of face correspondences in binary. It takes a map containing FaceCorrespondenceData objects a vector the corresponding shapes ordered by ID and the filename as arguments.
    static void exportFaceCorrespondencesBinary(
                                               HashMap<FaceCorrespondenceData*, bool>&  faceCorrespondences,
                                               vector<Shape*>&                          shapesOrderedById,
                                               string                                   filename
                                            );
    
    /// Reads two vectors of point and face correspondences respectively from a binary file.
    static void importCorrespondencesBinary(string                                   filename,
                                           int&                                      lastInsertCorrespondenceID_,
                                           vector<CorrespondenceData*>&         pointCorrespondences,
                                           vector<CorrespondenceData*>&          faceCorrespondences,
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
