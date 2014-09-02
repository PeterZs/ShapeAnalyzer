//
//  MeshChecker.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 26.08.14.
//
//

#include "MeshChecker.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
MeshChecker::MeshChecker(Shape* shape) : shape_(shape) {
    createHalfEdgeStructure();
}


///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
bool MeshChecker::checkForBorders() {
    if(halfEdges_.empty()) {
        createHalfEdgeStructure();
    }
    
    return checkForBorders(halfEdges_);
}


///////////////////////////////////////////////////////////////////////////////
bool MeshChecker::checkForBorders(vector<pair<vtkIdType, vtkIdType> >* borders) {
    if(halfEdges_.empty()) {
        createHalfEdgeStructure();
    }
    
    return checkForBorders(halfEdges_, borders);
}


///////////////////////////////////////////////////////////////////////////////
bool MeshChecker::checkForBorders(vector<vector<int> >&                 halfedges,
                                  vector<pair<vtkIdType, vtkIdType> >*  borders) {
    
    bool borderFound = false;
    
    for (int i = 0; i < halfedges.size(); i++) {
        for (int j = 0; j < i; j++) {
            // check symmetry
            if(halfedges[i][j] != halfedges[j][i]) {
                borderFound = true;
                
                // add border ids if requested
                if (borders != nullptr) {
                    // all borders are oriented the same way
                    if (halfedges[i][j] > halfedges[j][i]) {
                        borders->push_back(make_pair(i, j));
                    } else {
                        borders->push_back(make_pair(j, i));
                    }
                }
            }
        }
    }
    
    return borderFound;
}


///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void MeshChecker::createHalfEdgeStructure() {
    vtkIdType numberOfVertices = shape_->getPolyData()->GetPoints()->GetNumberOfPoints();
    
    halfEdges_.reserve(numberOfVertices);
    
    // create fake array with 0s everywhere
    for (int i = 0; i < numberOfVertices; i++) {
        halfEdges_[i] = vector<int>(numberOfVertices, 0);
    }
    
    // raise value [i, j] for every edge of an cell between the vertices i and j
    for (vtkIdType i = 0; i < shape_->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = shape_->getPolyData()->GetCell(i)->GetPointIds();
        
        // first n - 1 edges
        for(vtkIdType j = 0; j < ids->GetNumberOfIds() - 1; j++) {
            halfEdges_[ids->GetId(j)][ids->GetId(j+1)]++;
        }
        
        // edge from n to 0
        halfEdges_[ids->GetId(ids->GetNumberOfIds() - 1)][ids->GetId(0)]++;
    }
}