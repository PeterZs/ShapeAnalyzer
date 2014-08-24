//
//  CorrespondenceData.h
//  ShapeAnalyzer
//
//  Abstract Class for Correspondences that are shown in the GUI. 
//

#ifndef __ShapeAnalyzer__Correspondence__
#define __ShapeAnalyzer__Correspondence__

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkType.h>
#include <vtkLine.h>
#include <vtkLinearTransform.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>

#include "../HashMap.h"
#include "../Shape.h"

#include "CorrespondenceData.h"

#include <vector>
#include <algorithm>

using namespace std;

class Correspondence {

public:
    virtual ~Correspondence() {
        actors_.clear();
    }
    
    void initialize();
    
    //apply current user transformation of shape to the reference points that belong to the transformed shape and update line source with the transformed point as well as the actors belonging to the corresponding faces or points.
    void transform(Shape* shape);
    
    void setSelected(bool selected);

    //add another shape + face ID (or vertex ID) pair to this correspondence returns 1 if shape id combi has successfully been added (i.e. shape has not been selected twice). Returns 0 if shape equals last added shape and updates coordinates. Returns -1 if shape equals another shape that has already been added and is not equals to last added shape
    int addShape(Shape* shape, vtkIdType);
    
    void addShapes(unordered_map<Shape*, vtkIdType>& shapes);
    
    // getters
    vtkActor* getLinesActor() {
        return linesActor_;
    }
    
    vector<Shape*>& getShapes() {
        return shapes_;
    }
    
    CorrespondenceData* getData() {
        return data_;
    }
    
    string getLabel() {
        return label_;
    }
    
    void setLabel(string label) {
        label_ = label;
    }
    
    // vtk
    void removeFromRenderer();
    void addToRenderer();
    
protected:
    //protected contructor since class is abstract
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, string label, CorrespondenceData* data);
    Correspondence(vtkSmartPointer<vtkRenderer> renderer, string label, CorrespondenceData* data, HashMap<vtkActor*, Shape*>& shapes);
    
    virtual void initializeActor(vtkSmartPointer<vtkActor> actor, Shape* shape, vtkIdType) = 0;
    
    virtual void getCorrespondencePoint(double point[3], Shape* shape, vtkIdType) = 0;
    
    vtkSmartPointer<vtkRenderer>    renderer_;
    string                          label_; //label displayed in the correspondence list
    CorrespondenceData*             data_; //contains list of shape IDs and list of face or vertex IDs and the unique ID
    vector<Shape*>                  shapes_;

    
    //vtk visualization stuff
    vector<vtkSmartPointer<vtkActor> >  actors_; // actors representing highlighted vertex or face (part of this correspondence) on corresponding shape. (Ordering corresponds to ordering of shapes)
    
    vtkSmartPointer<vtkPoints>          lineReferencePoints_; // save the reference line coordinates to be able to transform lines if shapes are transformed

    vtkSmartPointer<vtkPolyData>        linesPolyData_; // lines polydata. The points inside poly data have the same ordering as shapes.
    vtkSmartPointer<vtkPolyDataMapper>  linesMapper_;
    vtkSmartPointer<vtkActor>           linesActor_; // lines actor
};

#endif /* defined(__ShapeAnalyzer__Correspondence__) */
