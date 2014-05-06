//
//  CorrespondencePicker.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 25.04.14.
//
//

#ifndef CorrespondencePicker_H
#define CorrespondencePicker_H


#include <vtkActor.h>
#include <vtkExtractSelection.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPointPlacer.h>

#include <assert.h>

#include "Shape.h"
#include "Correspondence.h"

//Class responsible for creating a correspondence
class CorrespondencePicker {
public:
    CorrespondencePicker(vtkRenderer* renderer) : renderer_(renderer) {
        selectedFaceActor_ = vtkSmartPointer<vtkActor>::New();
        selectedFaceMapper_ = vtkSmartPointer<vtkDataSetMapper>::New();
        lineActor_ = vtkSmartPointer<vtkActor>::New();
        lineMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    bool pick(Correspondence** correspondence, Shape *shape, vtkIdType cellId);
    void mouseMoveHandler(int x, int y);
    void clearSelection(Shape* shape);
    void clearSelection();
private:
    vtkSmartPointer<vtkUnstructuredGrid> getSelectionGrid(Shape *shape, vtkIdType cellId);
    vtkSmartPointer<vtkActor> createFaceActorFromGrid(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkLinearTransform* t);
    
    
    bool waitForSelection_; //flag indicating that a face has been selected on first shape. Wait for selection of corresponding face on another shape
    Shape* selectedShape_; //Pointer to shape that was selected in add-Correspondeces-Mode and therefore has the green triangle on it. Required to be declared at class level to delete green triangle in case the selected shape is deleted in method deleteShape(int i)
    
    vtkSmartPointer<vtkActor> selectedFaceActor_; // actor representing green triangle on selected shape
    vtkSmartPointer<vtkDataSetMapper> selectedFaceMapper_;
    vtkSmartPointer<vtkTriangle> face1_; //Remember first selection.
    Shape* shape1_; //Remember first selection.
    vtkSmartPointer<vtkActor> face1Actor_;
    
    //vtk visualization stuff
    vtkSmartPointer<vtkPolyData> linePolyData_; //polydata that is visualized
    vtkSmartPointer<vtkActor> lineActor_;
    vtkSmartPointer<vtkPolyDataMapper> lineMapper_;
    
    vtkRenderer* renderer_;
};

#endif /* defined(CorrespondencePicker_H) */
