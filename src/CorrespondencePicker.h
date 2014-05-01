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

#include "Shape.h"
#include "Correspondence.h"

//Class responsible for creating a correspondence
class CorrespondencePicker {
public:
    CorrespondencePicker(vtkRenderer* renderer) : renderer_(renderer) {
        selectedNodeActor_ = vtkSmartPointer<vtkActor>::New();
        selectedNodeMapper_ = vtkSmartPointer<vtkDataSetMapper>::New();
        lineActor_ = vtkSmartPointer<vtkActor>::New();
        lineMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    bool pick(Correspondence** correspondence, Shape *shape, vtkIdType cellId);
    void mouseMoveHandler(int x, int y);
    void clearSelection(Shape* shape);
    void clearSelection();
private:
    vtkSmartPointer<vtkUnstructuredGrid> getSelectedNodeGrid(Shape *shape, vtkIdType cellId);
    vtkSmartPointer<vtkActor> createTriangleActorFromGrid(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkLinearTransform* t);
    
    
    bool waitForSelection_; //flag indicating that a triangle has been selected on first shape. Wait for selection of corresponding triangle on another shape
    Shape* selectedShape_; //Pointer to shape that was selected in add-Correspondeces-Mode and therefore has the green triangle on it. Required to be declared in the class to delete green triangle in case the selected shape is deleted in method deleteShape(int i)
    
    vtkSmartPointer<vtkActor> selectedNodeActor_; // actor representing green triangle on selected shape
    vtkSmartPointer<vtkDataSetMapper> selectedNodeMapper_;
    vtkSmartPointer<vtkTriangle> triangle1_; //Remember first selection.
    Shape* shape1_; //Remember first selection.
    vtkSmartPointer<vtkActor> triangle1Actor_;
    
    //vtk visualization stuff
    vtkSmartPointer<vtkPolyData> linePolyData_; //polydata that is visualized
    vtkSmartPointer<vtkActor> lineActor_;
    vtkSmartPointer<vtkPolyDataMapper> lineMapper_;
    
    vtkRenderer* renderer_;
};

#endif /* defined(CorrespondencePicker_H) */
