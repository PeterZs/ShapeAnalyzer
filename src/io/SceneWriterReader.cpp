//
//  SceneWriterReader.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#include "SceneWriterReader.h"


///////////////////////////////////////////////////////////////////////////////
void SceneWriterReader::importSceneBinary(
                                          string filename,
                                          vtkSmartPointer<vtkRenderer> renderer,
                                          int& lastInsertShapeID,
                                          vector<Shape*>& shapes,
                                          int& lastInsertCorrespondenceID,
                                          HashMap<PointCorrespondenceData*, PointCorrespondence*>& pointCorrespondences,
                                          HashMap<FaceCorrespondenceData*, FaceCorrespondence*>& faceCorrespondences
                                          ) {

    
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
        //Shape* shape = new Shape(renderer);
        
        //shape->readBinary(is);
        //shapes.push_back(shape);
        //shapesByActor.insert(shape->getActor(), shape);
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
        //shapes[i]->writeBinary(os);
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
    
    HashMap<vtkActor*, Shape*> shapesByActor;
    //read shapes
    for(unsigned int i = 0; i < numberOfShapes; i++) {
        //Shape* shape = new Shape(renderer);
        
        //shape->readASCII(is);
        //shapes.push_back(shape);
        //shapesByActor.insert(shape->getActor(), shape);
    }
    
    // read camera attributes
    double position1, position2, position3;
    
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> position1;
        ss >> position2;
        ss >> position3;
    }
    renderer->GetActiveCamera()->SetPosition(position1, position2, position3);
    
    double windowCenter1, windowCenter2;
    
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> windowCenter1;
        ss >> windowCenter2;
    }
    renderer->GetActiveCamera()->SetWindowCenter(windowCenter1, windowCenter2);
    
    double viewangle, eyeangle;
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> viewangle;
        ss >> eyeangle;
    }
    renderer->GetActiveCamera()->SetViewAngle(viewangle);
    renderer->GetActiveCamera()->SetEyeAngle(eyeangle);
    
    double* eyeposition = new double[3];
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> eyeposition[0];
        ss >> eyeposition[1];
        ss >> eyeposition[2];
    }
    renderer->GetActiveCamera()->SetEyePosition(eyeposition);
    
    double* screenLeftBottom = new double[3];
    double* screenRightBottom = new double[3];
    double* screenRightTop = new double[3];
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> screenLeftBottom[0];
        ss >> screenLeftBottom[1];
        ss >> screenLeftBottom[2];
    }
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> screenRightBottom[0];
        ss >> screenRightBottom[1];
        ss >> screenRightBottom[2];
    }
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> screenRightTop[0];
        ss >> screenRightTop[1];
        ss >> screenRightTop[2];
    }
    renderer->GetActiveCamera()->SetScreenBottomLeft(screenLeftBottom);
    renderer->GetActiveCamera()->SetScreenBottomRight(screenRightBottom);
    renderer->GetActiveCamera()->SetScreenTopRight(screenRightTop);
    
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
            correspondence = new PointCorrespondence(renderer, data, shapesByActor);
            correspondence->initialize();
        }
        
        pointCorrespondences.insert(data, correspondence);
    }
    
    //read face correspondences
    int numberOfFaceCorrespondences;
    {
        stringstream ss;
        getline(is, line);
        ss << line;
        ss >> numberOfFaceCorrespondences;
    }
    
    for(int i = 0; i < numberOfFaceCorrespondences; i++) {
        stringstream ss;
        getline(is, line);
        ss << line;
        
        int id;
        ss >> id;
        
        FaceCorrespondenceData* data = new FaceCorrespondenceData(id);
        
        bool visible;
        ss >> visible;
        
        int numberOfShapes;
        ss >> numberOfShapes;
        
        for(int j = 0; j < numberOfShapes; j++) {
            int shapeId;
            int faceId;
            ss >> shapeId;
            ss >> faceId;
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
void SceneWriterReader::exportSceneASCII
            (string filename,
             vtkSmartPointer<vtkRenderer> renderer,
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
        //shapes[i]->writeASCII(os);
    }
    
    // write camera attributes
    double position1, position2, position3;
    renderer->GetActiveCamera()->GetPosition(position1, position2, position3);
    
    os << position1 << "\t" << position2 << "\t" << position3 << endl;
    
    double windowCenter1, windowCenter2;
    renderer->GetActiveCamera()->GetWindowCenter(windowCenter1, windowCenter2);
    os << windowCenter1 << "\t" << windowCenter2 << endl;
    
    double viewangle = renderer->GetActiveCamera()->GetViewAngle();
    double eyeangle = renderer->GetActiveCamera()->GetEyeAngle();
    os << viewangle << "\t" << eyeangle << endl;
    
    double* eyePosition = new double[3];
    renderer->GetActiveCamera()->GetEyePosition(eyePosition);
    os << eyePosition[0] << "\t" << eyePosition[1] << "\t" << eyePosition[2] << endl;
    delete eyePosition;
    
    double* screenLeftBottom = new double[3];
    double* screenRightBottom = new double[3];
    double* screenRightTop = new double[3];
    renderer->GetActiveCamera()->GetScreenBottomLeft(screenLeftBottom);
    renderer->GetActiveCamera()->GetScreenBottomRight(screenRightBottom);
    renderer->GetActiveCamera()->GetScreenTopRight(screenRightTop);
    os << screenLeftBottom[0] << "\t" << screenLeftBottom[1] << "\t" << screenLeftBottom[2] << endl;
    os << screenRightBottom[0] << "\t" << screenRightBottom[1] << "\t" << screenRightBottom[2] << endl;
    os << screenRightTop[0] << "\t" << screenRightTop[1] << "\t" << screenRightTop[2] << endl;
    delete screenLeftBottom;
    delete screenRightBottom;
    delete screenRightTop;
    
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


/////////////////////////////////////////////////////////////////////////////////
//ostream& Shape::writeBinary(ostream& os) {
//    //write shape ID.
//    int64_t id = (int64_t) id_;
//    os.write(reinterpret_cast<const char*>(&id), sizeof(int64_t));
//    
//    const char* name = name_.c_str();
//    
//    int64_t length = name_.length();
//    os.write(reinterpret_cast<const char*>(&length), sizeof(int64_t));
//    os.write(name, length*sizeof(char));
//    
//    vtkSmartPointer<vtkMatrix4x4> transform = actor_->GetUserMatrix();
//    //if user has not transformed shape write identity.
//    if(transform == nullptr) {
//        transform = vtkSmartPointer<vtkMatrix4x4>::New();
//        transform->Identity();
//    }
//    
//    //write elements of 4x4 matrix
//    for(int i = 0; i < 4; i++) {
//        for(int j = 0; j < 4; j++) {
//            double value = transform->GetElement(i, j);
//            os.write(reinterpret_cast<const char*>(&value), sizeof(double));
//        }
//    }
//    
//    //write number of points.
//    int64_t numberOfPoints = (int64_t) polyData_->GetNumberOfPoints();
//    os.write(reinterpret_cast<const char*>(&numberOfPoints), sizeof(int64_t));
//    
//    
//    //write number of faces.
//    int64_t numberOfFaces = (int64_t) polyData_->GetNumberOfCells();
//    os.write(reinterpret_cast<const char*>(&numberOfFaces), sizeof(int64_t));
//    
//    //write points.
//    for(int64_t i = 0; i < numberOfPoints; i++) {
//        double point[3];
//        polyData_->GetPoints()->GetPoint(i, point);
//        
//        os.write(reinterpret_cast<const char*>(point), 3 * sizeof(double));
//    }
//    
//    //write faces.
//    for(int64_t i = 0; i < numberOfFaces; i++) {
//        for(int j = 0; j < 3; j++) {
//            int64_t pointId = (int64_t) polyData_->GetCell(i)->GetPointId(j);
//            os.write(reinterpret_cast<const char*>(&pointId), sizeof(int64_t));
//        }
//    }
//    
//    return os;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//istream& Shape::readBinary(istream& is) {
//    //read shape ID
//    int64_t id;
//    is.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
//    id_ = id;
//    
//    int64_t length;
//    is.read(reinterpret_cast<char*>(&length), sizeof(int64_t));
//    char* name = new char[length+1];
//    is.read(name, length*sizeof(char));
//    name[length] = '\0';
//    
//    name_ = name;
//    delete [] name;
//    
//    //read user transform. Set user transform in actor after poly data has been read and shape has been initialized and therefore actor_ != nullptr
//    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
//    for(int i = 0; i < 4; i++) {
//        for(int j = 0; j < 4; j++) {
//            double value;
//            is.read(reinterpret_cast<char*>(&value), sizeof(double));
//            matrix->SetElement(i, j, value);
//        }
//    }
//    
//    //read number of points.
//    int64_t numberOfPoints;
//    is.read(reinterpret_cast<char*>(&numberOfPoints), sizeof(int64_t));
//    //read number of faces.
//    int64_t numberOfFaces;
//    is.read(reinterpret_cast<char*>(&numberOfFaces), sizeof(int64_t));
//    
//    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
//    
//    //read points
//    for(int64_t i = 0; i < numberOfPoints; i++) {
//        double point[3];
//        is.read(reinterpret_cast<char*>(point), sizeof(double) * 3);
//        points->InsertNextPoint(point);
//    }
//    
//    //read faces
//    for(int64_t i = 0; i < numberOfFaces; i++) {
//        vtkSmartPointer<vtkTriangle> face = vtkSmartPointer<vtkTriangle>::New();
//        for(int j = 0; j < 3; j++) {
//            int64_t pointId;
//            is.read(reinterpret_cast<char*>(&pointId), sizeof(int64_t));
//            
//            face->GetPointIds()->SetId(j, pointId);
//        }
//        polys->InsertNextCell(face);
//    }
//    polyData_ = vtkSmartPointer<vtkPolyData>::New();
//    polyData_->SetPoints(points);
//    polyData_->SetPolys(polys);
//    
//    initialize();
//    
//    actor_->SetUserMatrix(matrix);
//    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//    transform->SetMatrix(matrix);
//    static_cast<vtkBoxRepresentation*>(boxWidget_->GetRepresentation())->SetTransform(transform);
//    return is;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//ostream& Shape::writeASCII(ostream& os) {
//    os << id_<< endl;
//    os << name_ << endl;
//    
//    vtkSmartPointer<vtkMatrix4x4> transform = actor_->GetUserMatrix();
//    if(transform == nullptr) {
//        transform = vtkSmartPointer<vtkMatrix4x4>::New();
//        transform->Identity();
//    }
//    
//    for(int i = 0; i < 4; i++) {
//        for(int j = 0; j < 4; j++) {
//            os << transform->GetElement(i, j) << (j != 3 ? "\t" : "");
//        }
//        os << endl;
//    }
//    
//    os << polyData_->GetNumberOfPoints() << "\t" << polyData_->GetNumberOfCells() << endl;
//    
//    for(vtkIdType i = 0; i < polyData_->GetNumberOfPoints(); i++) {
//        double point[3];
//        polyData_->GetPoints()->GetPoint(i, point);
//        os << point[0] << "\t" << point[1] << "\t" << point[2] << endl;
//    }
//    
//    for(vtkIdType i = 0; i < polyData_->GetNumberOfCells(); i++) {
//        os << polyData_->GetCell(i)->GetPointId(0) << "\t" << polyData_->GetCell(i)->GetPointId(1) << "\t" << polyData_->GetCell(i)->GetPointId(2) << endl;
//    }
//    
//    return os;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//istream& Shape::readASCII(istream& is) {
//    string line;
//    
//    //read shape ID.
//    {
//        getline(is, line);
//        stringstream ss;
//        ss << line;
//        ss >> id_;
//    }
//    
//    {
//        getline(is, line);
//        stringstream ss;
//        ss << line;
//        ss >> name_;
//    }
//    
//    //read user transform.
//    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
//    for(int i = 0; i < 4; i++) {
//        getline(is, line);
//        stringstream ss;
//        ss << line;
//        for(int j = 0; j < 4; j++) {
//            double value;
//            ss >> value;
//            matrix->SetElement(i, j, value);
//        }
//    }
//    
//    //read number of points, number of faces
//    vtkIdType numberOfPoints, numberOfFaces;
//    {
//        getline(is, line);
//        stringstream ss;
//        ss << line;
//        ss >> numberOfPoints >> numberOfFaces;
//    }
//    
//    //read points, faces
//    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
//    
//    for(vtkIdType i = 0; i < numberOfPoints; i++) {
//        getline(is, line);
//        stringstream ss;
//        ss << line;
//        double point[3];
//        ss >> point[0] >> point[1] >> point[2];
//        
//        points->InsertNextPoint(point);
//        
//    }
//    
//    for(vtkIdType i = 0; i < numberOfFaces; i++) {
//        getline(is, line);
//        stringstream ss;
//        ss << line;
//        
//        vtkSmartPointer<vtkTriangle> face = vtkSmartPointer<vtkTriangle>::New();
//        
//        for(int j = 0; j < 3; j++) {
//            vtkIdType pointId;
//            ss >> pointId;
//            face->GetPointIds()->SetId(j, pointId);
//        }
//        polys->InsertNextCell(face);
//    }
//    
//    polyData_ = vtkSmartPointer<vtkPolyData>::New();
//    polyData_->SetPoints(points);
//    polyData_->SetPolys(polys);
//    
//    initialize();
//    
//    actor_->SetUserMatrix(matrix);
//    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//    transform->SetMatrix(matrix);
//    static_cast<vtkBoxRepresentation*>(boxWidget_->GetRepresentation())->SetTransform(transform);
//    
//    return is;
//}

