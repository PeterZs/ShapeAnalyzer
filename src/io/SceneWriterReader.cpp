//
//  SceneWriterReader.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#include "SceneWriterReader.h"


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::importSceneBinary(string filename, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, vector<Shape*>& shapes, int& lastInsertCorrespondenceID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences) {

    
    //open input file stream in binary mode
    ifstream is(filename, ios::binary);
    
    //read number of shapes
    int64_t numberOfShapes;
    is.read(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
    
    
    //read last insert shape ID
    is.read(reinterpret_cast<char*>(&lastInsertShapeID), sizeof(int64_t));
    
    
    HashMap<vtkActor*, Shape*> shapesByActor;
    //read shapes
    for(int64_t i = 0; i < numberOfShapes; i++) {
        Shape* shape = new Shape(renderer);
        
        shape->readBinary(is);
        shapes.push_back(shape);
        shapesByActor.insert(shape->getActor(), shape);
    }
    
    //read last insert correspondence ID
    is.read(reinterpret_cast<char*>(&lastInsertCorrespondenceID), sizeof(int64_t));
    

    //read point correspondences
    int64_t numberOfPointCorrespondences;
    
    
    is.read(reinterpret_cast<char*>(&numberOfPointCorrespondences), sizeof(int64_t));

    for(int64_t i = 0; i < numberOfPointCorrespondences; i++) {
        int64_t id;
        is.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
        PointCorrespondenceData* data = new PointCorrespondenceData(id);
        
        bool visible;
        is.read(reinterpret_cast<char*>(&visible), sizeof(bool));

        int64_t numberOfShapes;
        is.read(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
        
        for(int64_t j = 0; j < numberOfShapes; j++) {
            int64_t shapeId;
            int64_t faceId;
            is.read(reinterpret_cast<char*>(&shapeId), sizeof(int64_t));
            is.read(reinterpret_cast<char*>(&faceId), sizeof(int64_t));
            data->addShape(shapeId, faceId);
        }
        
        PointCorrespondence* correspondence = nullptr;
        if(visible) {
            correspondence = new PointCorrespondence(renderer, data, shapesByActor);
            correspondence->initialize();
        }
        
        pointCorrespondences.insert(data, correspondence);
    }
    
    //read face correspondences
    int64_t numberOfFaceCorrespondences;
    is.read(reinterpret_cast<char*>(&numberOfFaceCorrespondences), sizeof(int64_t));

    for(int64_t i = 0; i < numberOfFaceCorrespondences; i++) {
        int64_t id;
        is.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
        FaceCorrespondenceData* data = new FaceCorrespondenceData(id);
        
        bool visible;
        is.read(reinterpret_cast<char*>(&visible), sizeof(bool));
        
        int64_t numberOfShapes;
        is.read(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
        
        for(int64_t j = 0; j < numberOfShapes; j++) {
            int64_t shapeId;
            int64_t faceId;
            is.read(reinterpret_cast<char*>(&shapeId), sizeof(int64_t));
            is.read(reinterpret_cast<char*>(&faceId), sizeof(int64_t));
            data->addShape(shapeId, faceId);
        }
        
        FaceCorrespondence* correspondence = nullptr;
        if(visible) {
            correspondence = new FaceCorrespondence(renderer, data, shapesByActor);
            correspondence->initialize();
        }
        
        faceCorrespondences.insert(data, correspondence);
    }
    is.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportSceneBinary(string filename, vector<Shape*>& shapes, int lastInsertShapeID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences, int lastInsertCorrespondenceID) {
    ofstream os(filename, ios::binary);
    int64_t numberOfShapes = (int64_t) shapes.size();
    os.write(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
    
    int64_t lastInsertSID = (int64_t) lastInsertShapeID;
    os.write(reinterpret_cast<char*>(&lastInsertSID), sizeof(int64_t));
    
    // write shapes
    for(int i = 0; i < shapes.size(); i++) {
        shapes[i]->writeBinary(os);
    }
    
    // write last insert correspondence id
    int64_t lastInsertCID = (int64_t) lastInsertCorrespondenceID;
    os.write(reinterpret_cast<char*>(&lastInsertCID), sizeof(int64_t));
    
    // write point correspondences
    int64_t numberOfPointCorrespondences = (int64_t) pointCorrespondences.size();
    os.write(reinterpret_cast<char*>(&numberOfPointCorrespondences), sizeof(int64_t));
    
    
    for(HashMap<PointCorrespondenceData*, PointCorrespondence*>::iterator it = pointCorrespondences.begin(); it != pointCorrespondences.end(); it++) {
        int64_t id = it->first->getId();
        os.write(reinterpret_cast<char*>(&id), sizeof(int64_t));
        
        bool visible = it->second != nullptr;
        os.write(reinterpret_cast<char*>(&visible), sizeof(bool));
        
        int64_t numberOfShapes = it->first->size();
        os.write(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
        
        for(int64_t i = 0; i < numberOfShapes; i++) {
            int64_t shapeId = it->first->getShapeIds()[i];
            int64_t pointId = it->first->getCorrespondingIds()[i];
            os.write(reinterpret_cast<char*>(&shapeId), sizeof(int64_t));
            os.write(reinterpret_cast<char*>(&pointId), sizeof(int64_t));
        }
    }

    // write face correspondences
    int64_t numberOfFaceCorrespondences = (int64_t) faceCorrespondences.size();
    os.write(reinterpret_cast<char*>(&numberOfFaceCorrespondences), sizeof(int64_t));
    
    
    for(HashMap<FaceCorrespondenceData*, FaceCorrespondence*>::iterator it = faceCorrespondences.begin(); it != faceCorrespondences.end(); it++) {
        int64_t id = it->first->getId();
        os.write(reinterpret_cast<char*>(&id), sizeof(int64_t));
        
        bool visible = it->second != nullptr;
        os.write(reinterpret_cast<char*>(&visible), sizeof(bool));
        
        int64_t numberOfShapes = it->first->size();
        os.write(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
        
        for(int64_t i = 0; i < numberOfShapes; i++) {
            int64_t shapeId = it->first->getShapeIds()[i];
            int64_t faceId = it->first->getCorrespondingIds()[i];
            os.write(reinterpret_cast<char*>(&shapeId), sizeof(int64_t));
            os.write(reinterpret_cast<char*>(&faceId), sizeof(int64_t));
        }
    }
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::importSceneASCII(string filename, vtkSmartPointer<vtkRenderer> renderer, int& lastInsertShapeID, vector<Shape*>& shapes, int& lastInsertCorrespondenceID, HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences, HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences) {
    ifstream is(filename);
    
    string line;
    
    // read shapes
    unsigned int numberOfShapes;
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> numberOfShapes;
    }
    
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> lastInsertShapeID;
    }
    
    for(unsigned int i = 0; i < numberOfShapes; i++) {
        Shape* shape = new Shape(renderer);
        shape->readASCII(is);
        shapes.push_back(shape);
        
    }
    
    // read correspondences
    
    //read last insert correspondence ID
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> lastInsertCorrespondenceID;
    }
    
    
    //read point correspondences
    int numberOfPointCorrespondences;
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> numberOfPointCorrespondences;
    }
    
    for(int i = 0; i < numberOfPointCorrespondences; i++) {
        stringstream ss;
        getline(is, line);
        ss << line;
        
        int id;
        ss >> id;
        
        PointCorrespondenceData* data = new PointCorrespondenceData(id);
        
        bool visible;
        ss >> visible;
        
        int numberOfShapes;
        ss >> numberOfShapes;
        
        for(int j = 0; j < numberOfShapes; j++) {
            int shapeId;
            int pointId;
            ss >> shapeId;
            ss >> pointId;
            data->addShape(shapeId, pointId);
        }
        
        PointCorrespondence* correspondence = nullptr;
        if(visible) {
        //    correspondence = new PointCorrespondence(renderer, data, shapesByActor);
        //    correspondence->initialize();
        }
        
        pointCorrespondences.insert(data, correspondence);
    }
    
    //read face correspondences
    int64_t numberOfFaceCorrespondences;
    is.read(reinterpret_cast<char*>(&numberOfFaceCorrespondences), sizeof(int64_t));
    
    for(int64_t i = 0; i < numberOfFaceCorrespondences; i++) {
        int64_t id;
        is.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
        FaceCorrespondenceData* data = new FaceCorrespondenceData(id);
        
        bool visible;
        is.read(reinterpret_cast<char*>(&visible), sizeof(bool));
        
        int64_t numberOfShapes;
        is.read(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
        
        for(int64_t j = 0; j < numberOfShapes; j++) {
            int64_t shapeId;
            int64_t faceId;
            is.read(reinterpret_cast<char*>(&shapeId), sizeof(int64_t));
            is.read(reinterpret_cast<char*>(&faceId), sizeof(int64_t));
            data->addShape(shapeId, faceId);
        }
        
        FaceCorrespondence* correspondence = nullptr;
        if(visible) {
        //    correspondence = new FaceCorrespondence(renderer, data, shapesByActor);
        //    correspondence->initialize();
        }
        
        faceCorrespondences.insert(data, correspondence);
        
    }
    
    is.close();
}

///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportSceneASCII
            (string filename,
             vector<Shape*>& shapes,
             int lastInsertShapeID,
             HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences,
             HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences,
             int lastInsertCorrespondenceID)
{
    ofstream os(filename);
    
    // write shapes
    os << shapes.size() << endl;
    os << lastInsertShapeID << endl;
    for(int i = 0; i < shapes.size(); i++) {
        shapes[i]->writeASCII(os);
    }
    
    // write last insert correspondence id
    os << lastInsertCorrespondenceID;
    os << endl;
    
    // write point correspondences
    int numberOfPointCorrespondences = pointCorrespondences.size();
    os << numberOfPointCorrespondences;
    os << endl;
    
    for(auto it = pointCorrespondences.begin(); it != pointCorrespondences.end(); it++) {
        int id = it->first->getId();
        os << id << "\t";
        
        bool visible = it->second != nullptr;
        os << visible << "\t";
        
        int numberOfShapes = it->first->size();
        os << numberOfShapes << "\t";
        
        for(int i = 0; i < numberOfShapes; i++) {
            int shapeId = it->first->getShapeIds()[i];
            int pointId = it->first->getCorrespondingIds()[i];
            os << shapeId << "\t";
            os << pointId << "\t";
        }
        
        os << endl;
    }
    
    // write face correspondences
    int numberOfFaceCorrespondences = faceCorrespondences.size();
    os << numberOfFaceCorrespondences;
    os << endl;
    
    for(auto it = faceCorrespondences.begin(); it != faceCorrespondences.end(); it++) {
        int id = it->first->getId();
        os << id << "\t";
        
        bool visible = it->second != nullptr;
        os << visible << "\t";
        
        int numberOfShapes = it->first->size();
        os << numberOfShapes << "\t";
        
        for(int i = 0; i < numberOfShapes; i++) {
            int shapeId = it->first->getShapeIds()[i];
            int faceId = it->first->getCorrespondingIds()[i];
            os << shapeId << "\t";
            os << faceId << "\t";
        }
        
        os << endl;
    }
    
    os.close();
}



///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportCorrespondencesASCII(HashMap<CorrespondenceData*, bool>&   correspondences,
                                        vector<Shape*>&                             shapesOrderedById,
                                        ostream&                                    os)
{
    os << shapesOrderedById.size() << " " << correspondences.size() << endl;
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
void SceneWriterReader::exportPointCorrespondencesASCII(HashMap<PointCorrespondenceData*, bool>& correspondences,
                                             vector<Shape*>&                                shapesOrderedById,
                                             string                                         filename)
{
    ofstream os(filename);
    
    // basic information, point correspondences, number of shapes
    // number of correspondences
    os << "P ";
    
    exportCorrespondencesASCII((HashMap<CorrespondenceData*, bool>&) correspondences, shapesOrderedById, os);
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportFaceCorrespondencesASCII(HashMap<FaceCorrespondenceData*, bool>&   correspondences,
                                            vector<Shape*>&                                 shapesOrderedById,
                                            string                                          filename)
{
    ofstream os(filename);
    
    // basic information, face correspondences, number of shapes
    // number of correspondences
    os << "F ";
    
    exportCorrespondencesASCII((HashMap<CorrespondenceData*, bool>&) correspondences, shapesOrderedById, os);
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::importCorrespondencesASCII(string                            filename,
                                      int&                                      lastInsertCorrespondenceID_,
                                      vector<CorrespondenceData*>&         pointCorrespondences,
                                      vector<CorrespondenceData*>&          faceCorrespondences,
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
        QString label = QString::number(shapesOrderedById[j]->getId());
        label.append(QString::fromStdString(":"+shapesOrderedById[j]->getName()));
        labels << label;
    }
    
    vector<Shape*> correspondingShapes = vector<Shape*>(0);
    
    // user chooses numberOfShapes many shapes to correspond
    unsigned counter = 1;
    for (int i = 0; i < numberOfShapes; i++) {        
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
            vtkIdType shapeId = chosen.split(':')[0].toInt();
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
        createCorrespondenceDataASCII(correspondingShapes, ss, data);
        
        if (type == 'F') {
            faceCorrespondences.push_back((FaceCorrespondenceData*) data);
        } else {
            pointCorrespondences.push_back((PointCorrespondenceData*) data);
        }
        lastInsertCorrespondenceID_++;
    }
}

///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::createCorrespondenceDataASCII(vector<Shape*>& shapes, stringstream& ss, CorrespondenceData* data) {
    for (auto it = shapes.begin(); it != shapes.end(); it++) {
        int id;
        ss >> id;
        if (id > -1) {
            data->addShape((*it)->getId(), id);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportCorrespondencesBinary(HashMap<CorrespondenceData*, bool>&        correspondences,
                                                   vector<Shape*>&                             shapesOrderedById,
                                                   ostream&                                    os)
{
    int64_t numberOfShapes = shapesOrderedById.size();
    os.write(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
    
    int64_t numberOfCorrespondences = correspondences.size();
    os.write(reinterpret_cast<char*>(&numberOfCorrespondences), sizeof(int64_t));
    
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
                    int64_t correspondingId = (int64_t) correspondingIds[j];
                    os.write(reinterpret_cast<char*>(&correspondingId), sizeof(int64_t));
                    contained = true;
                    break;
                }
            }
            // -1 indicates this shape is not in the correspondence
            if (!contained) {
                int64_t correspondingId = -1;
                os.write(reinterpret_cast<char*>(&correspondingId), sizeof(int64_t));
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportPointCorrespondencesBinary(HashMap<PointCorrespondenceData*, bool>& correspondences,
                                                        vector<Shape*>&                                shapesOrderedById,
                                                        string                                         filename)
{
    ofstream os(filename, ios::binary);
    
    // write type
    const char type = 'P';
    os.write(&type, sizeof(char));

    exportCorrespondencesBinary((HashMap<CorrespondenceData*, bool>&) correspondences, shapesOrderedById, os);
    
    os.close();
}


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::exportFaceCorrespondencesBinary(HashMap<FaceCorrespondenceData*, bool>&   correspondences,
                                                       vector<Shape*>&                                 shapesOrderedById,
                                                       string                                          filename)
{
    ofstream os(filename, ios::binary);
    
    // write type
    const char type = 'F';
    os.write(&type, sizeof(char));
    
    exportCorrespondencesBinary((HashMap<CorrespondenceData*, bool>&) correspondences, shapesOrderedById, os);
    
    os.close();
}



///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::importCorrespondencesBinary(string                            filename,
                                                   int&                                      lastInsertCorrespondenceID_,
                                                   vector<CorrespondenceData*>&         pointCorrespondences,
                                                   vector<CorrespondenceData*>&          faceCorrespondences,
                                                   vector<Shape*>&                           shapesOrderedById,
                                                   QWidget*                                  parentWidget)
{
    
    ifstream is(filename, ios::binary);

    // read 3 lines, type of correspondences, number of shapes, number of correspondences
    char type;
    int64_t numberOfShapes, numberOfCorrespondences;
    
    is.read(&type, sizeof(char));
    is.read(reinterpret_cast<char*>(&numberOfShapes), sizeof(int64_t));
    is.read(reinterpret_cast<char*>(&numberOfCorrespondences), sizeof(int64_t));
    
    
    // stop if there not enough shapes to match
    if (shapesOrderedById.size() < numberOfShapes) {
        //TODO throw reader exception here, catch by main program
        return;
    }
    
    QStringList labels;
    // add all shapes that have not been chosen yet
    for (int j = 0; j < shapesOrderedById.size(); j++) {
        QString label = QString::number(shapesOrderedById[j]->getId());
        label.append(QString::fromStdString(":"+shapesOrderedById[j]->getName()));
        labels << label;
    }
    
    vector<Shape*> correspondingShapes = vector<Shape*>(0);
    
    // user chooses numberOfShapes many shapes to correspond
    unsigned counter = 1;
    for (int i = 0; i < numberOfShapes; i++) {
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
            vtkIdType shapeId = chosen.split(':')[0].toInt();
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
        
        for (auto it = shapesOrderedById.begin(); it != shapesOrderedById.end(); it++) {
            int64_t id;

            is.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
            
            if (id > -1) {
                data->addShape((*it)->getId(), id);
            }
        }
        
        if (type == 'F') {
            faceCorrespondences.push_back((FaceCorrespondenceData*) data);
        } else {
            pointCorrespondences.push_back((PointCorrespondenceData*) data);
        }
        lastInsertCorrespondenceID_++;
    }
}