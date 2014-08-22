//
//  SceneWriterReader.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#include "SceneWriterReader.h"

///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportCorrespondences(HashMap<CorrespondenceData*, bool>&   correspondences,
                                        vector<Shape*>&                             shapesOrderedById,
                                        ostream&                                    os)
{
    // write down all correspondences
    for (auto it = correspondences.begin(); it != correspondences.end(); it++) {
        vector<vtkIdType> shapeIds = it->first->getShapeIds();
        vector<vtkIdType> correspondingIds = it->first->getCorrespondingIds();
        
        // get the same order of shapes for all lines
        for (int i = 0; i < shapesOrderedById.size(); i++) {
            bool contained = false;
            // check for each possible shape whether it is in this correspondences or not
            for (int j = 0; j < shapeIds.size(); j++) {
                if (shapeIds[j] == shapesOrderedById[i]->getId()) {
                    // data[i] is the id of the correspondence element on the shape[i]
                    os << correspondingIds[j] << " ";
                    contained = true;
                    break;
                }
            }
            // -1 indicates this shape is not in the correspondence
            if (!contained) {
                os << "-1 ";
            }
        }
        // one line for each correspondence
        os << endl;
    }
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportPointCorrespondences(HashMap<PointCorrespondenceData*, bool>& correspondences,
                                             vector<Shape*>&                                shapesOrderedById,
                                             string                                         filename)
{
    ofstream os(filename);
    
    // basic information, point correspondences, number of shapes
    // number of correspondences
    os << "P " << " " << shapesOrderedById.size() << " " << correspondences.size() << endl;
    
    exportCorrespondences((HashMap<CorrespondenceData*, bool>&) correspondences, shapesOrderedById, os);
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportFaceCorrespondences(HashMap<FaceCorrespondenceData*, bool>&   correspondences,
                                            vector<Shape*>&                                 shapesOrderedById,
                                            string                                          filename)
{
    ofstream os(filename);
    
    // basic information, face correspondences, number of shapes
    // number of correspondences
    os << "F " << shapesOrderedById.size() << " " << correspondences.size() << endl;
    
    exportCorrespondences((HashMap<CorrespondenceData*, bool>&) correspondences, shapesOrderedById, os);
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::importCorrespondences(string                            filename,
                                      int&                                      lastInsertCorrespondenceID_,
                                      HashMap<PointCorrespondenceData*, bool>&  pointCorrespondences,
                                      HashMap<FaceCorrespondenceData*, bool>&   faceCorrespondences,
                                      vector<Shape*>&                           shapesOrderedById,
                                      QWidget*                                  parentWidget)
{

    ifstream is(filename);
    string line;
    
    // read 3 lines, type of correspondences, number of shapes, number of correspondences
    char type;
    unsigned int numberOfShapes, numberOfCorrespondences;
    
    stringstream ss;
    getline(is, line);
    ss << line;
    ss >> type >> numberOfShapes >> numberOfCorrespondences;
    

    // stop if there not enough shapes to match
    if (shapesOrderedById.size() < numberOfShapes) {
        //TODO throw reader exception here, catch by main program
        return;
    }
    
    QStringList labels;
    // add all shapes that have not been chosen yet
    for (int j = 0; j < shapesOrderedById.size(); j++) {
        QString label = "Shape ";
        label.append(QString::number(shapesOrderedById[j]->getId()+1));
        labels << label;
    }
    
    vector<Shape*> correspondingShapes = vector<Shape*>(0);
    
    // user chooses numberOfShapes many shapes to correspond
    unsigned counter = 1;
    for (int i = 0; i < numberOfShapes; i++) {

        
        // TODO the nullptr is not good, the windows move done...
        bool ok;
        QString chosen = QInputDialog::getItem(parentWidget,
                                               QObject::tr("Choose a shape:"),
                                               QString(QObject::tr("Shape for column ")).append(QString::number(counter)),
                                               labels,
                                               0,
                                               true,
                                               &ok);
        if(!ok) {
            return;
        }
        
        Shape* shape = nullptr;
        // add chosen shape to vector
        for(int j = 0; j < shapesOrderedById.size(); j++) {
            vtkIdType shapeId = chosen.split(' ')[1].toInt()-1;
            if(shapesOrderedById[j]->getId() == shapeId) {
                shape = shapesOrderedById[j];
                break;
            }
        }
        
        correspondingShapes.push_back(shape);
        // delete chosen shape from map, so it can not be chosen again
        labels.removeOne(chosen);
        counter++;
    }
    
    // create correspondeneces
    for (int i = 0; i < numberOfCorrespondences; i++) {
        CorrespondenceData* data;
        if (type == 'F') {
            data = new FaceCorrespondenceData(lastInsertCorrespondenceID_);
        } else {
            data = new PointCorrespondenceData(lastInsertCorrespondenceID_);
        }
        
        stringstream ss;
        getline(is, line);
        ss << line;
        createCorrespondenceData(correspondingShapes, ss, data);
        
        if (type == 'F') {
            faceCorrespondences.add((FaceCorrespondenceData*) data, false);
        } else {
            pointCorrespondences.add((PointCorrespondenceData*) data, false);
        }
        lastInsertCorrespondenceID_++;
    }
}

///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::createCorrespondenceData(vector<Shape*>& shapes, stringstream& ss, CorrespondenceData* data) {
    for (auto it = shapes.begin(); it != shapes.end(); it++) {
        int id;
        ss >> id;
        if (id > -1) {
            data->addShape((*it)->getId(), id);
        }
    }
}