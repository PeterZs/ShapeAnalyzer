//
//  SceneReader.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 28.07.14.
//
//

#include "SceneReader.h"


void SceneReader::exportCorrespondences(
                                        Set<CorrespondenceData*, bool>* correspondences,
                                        Set<vtkActor*, Shape*>*         shapeIds,
                                        ofstream*                       os
                                        )
{
    // write down all correspondences
    for (auto it = correspondences->begin(); it != correspondences->end(); it++) {
        vector<vtkIdType> shapes = it->first->getShapes();
        vector<vtkIdType> data = it->first->getData();
        // get the same order of shapes for all lines
        for (auto shapeIt = shapeIds->begin(); shapeIt != shapeIds->end(); shapeIt++) {
            bool contained = false;
            // check for each possible shape if it is in this correspondences
            for (int i = 0; i < shapes.size(); i++) {
                if (shapes[i] == shapeIt->second->getId()) {
                    // data[i] is the id of the correspondence element on the shape[i]
                    *os << data[i] << " ";
                    contained = true;
                }
            }
            // -1 indicates this shape is not in the correspondence
            if (!contained) {
                *os << "-1 ";
            }
        }
        // one line for each correspondence
        *os << endl;
    }
}

void SceneReader::exportPointCorrespondences(
                                             Set<PointCorrespondenceData*, bool>*   correspondences,
                                             Set<vtkActor*, Shape*>*                shapes,
                                             string                                 filename
                                             )
{
    ofstream os(filename);
    
    os << "P ";
    os << shapes->size() << " ";
    os << correspondences->size() << endl;
    
    exportCorrespondences((Set<CorrespondenceData*, bool>*) correspondences, shapes, &os);
    
    os.close();
}

void SceneReader::exportFaceCorrespondences(
                                            Set<FaceCorrespondenceData*, bool>* correspondences,
                                            Set<vtkActor*, Shape*>*             shapes,
                                            string                              filename
                                            )
{
    ofstream os(filename);
    
    os << "F ";
    os << shapes->size() << " ";
    os << correspondences->size() << endl;
    
    exportCorrespondences((Set<CorrespondenceData*, bool>*) correspondences, shapes, &os);
    
    os.close();
}