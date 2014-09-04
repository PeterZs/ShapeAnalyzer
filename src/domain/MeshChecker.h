//
//  MeshChecker.h
//  ShapeAnalyzer
//
//  Created by Zorah on 26.08.14.
//
//

#ifndef __ShapeAnalyzer__MeshChecker__
#define __ShapeAnalyzer__MeshChecker__

#include <vtkType.h>
#include <vtkCell.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkIdList.h>

#include "Shape.h"

using namespace std;

class MeshChecker {
public:
    MeshChecker(Shape* shape);
    ~MeshChecker();
    
    bool checkForBorders(vector<pair<vtkIdType, vtkIdType> >* borders = nullptr);
    bool checkOrientation(vector<pair<vtkIdType, vtkIdType> >* unoriented = nullptr);
    bool checkTriangulation(vector<pair<vtkIdType, vtkIdType> >* nontriangles = nullptr);
    
    int checkNumberOfRegions();
    
private:
    void createHalfEdgeStructure();
    
    Shape*                  shape_;
    vector<vector<int>* >    halfEdges_;
};

#endif /* defined(__ShapeAnalyzer__MeshChecker__) */
