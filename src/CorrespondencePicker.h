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
#include <vtkSmartPointer.h>

#include "vtkShape.h"
#include "Correspondence.h"
#include "FaceCorrespondence.h"

//Class responsible for creating a correspondence
class CorrespondencePicker {
public:
    virtual ~CorrespondencePicker() {
        clearSelection();
    }
    
    //abstract function: to override in subclass
    bool pick(
              Correspondence**              correspondence,
              vtkSmartPointer<vtkShape>     shape,
              vtkIdType);
    
    
    void updateLine(int x, int y);
    void clearSelection(vtkSmartPointer<vtkShape> shape);
    void clearSelection();

protected:
    //protected contructor: abstract class
    CorrespondencePicker(vtkRenderer* renderer) : waitForSelection_(false), renderer_(renderer) {
        lineActor_ = vtkSmartPointer<vtkActor>::New();
        lineMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
        selectionActor_ = vtkSmartPointer<vtkActor>::New();
        selectionMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    }

    virtual vtkSmartPointer<vtkPolyData> getSelectionPolyData(
                                                              vtkSmartPointer<vtkShape>    shape,
                                                              vtkIdType                     selectionId
                                                              ) = 0;
    virtual void getSelectionPoint(vtkSmartPointer<vtkShape> shape, vtkIdType selectionId, double point[3]) = 0;
    
    virtual Correspondence* createCorrespondence(
                                                 vtkSmartPointer<vtkRenderer>   renderer,
                                                 vtkSmartPointer<vtkShape>      shape1,
                                                 vtkSmartPointer<vtkShape>      shape2,
                                                 vtkIdType                      lastSelectionId,
                                                 vtkSmartPointer<vtkActor>      actor1,
                                                 vtkSmartPointer<vtkActor>      actor2
                                                 ) = 0;
    virtual void createActor(
                             vtkActor*              actor,
                             vtkPolyDataMapper*     mapper,
                             vtkPolyData*           polyData,
                             vtkLinearTransform*    t
                             ) = 0;
    
    
    vtkIdType id1_; // id of point or cell that was selected first.
private:
    void initializeLine(double point1[3]);
    vtkSmartPointer<vtkActor> createActor(vtkPolyData* polyData, vtkLinearTransform* t);
    
    bool waitForSelection_; // flag indicating that a face or a point has been selected on first shape. Wait for selection of corresponding face or point on another shape
    
    vtkRenderer* renderer_;
    vtkSmartPointer<vtkShape> selectedShape_; // Pointer to shape that was selected in add-Correspondeces-Mode and therefore has the yellow triangle/point on it. Required to be declared at class level to delete yellow triangle/point in case the selected shape is deleted in method deleteShape(int i)
    
    vtkSmartPointer<vtkActor> selectionActor_; // actor representing green triangle on selected shape
    vtkSmartPointer<vtkPolyDataMapper> selectionMapper_;
    
    // Remember first selection.
    vtkSmartPointer<vtkShape> shape1_;
    vtkSmartPointer<vtkActor> actor1_; // actor representing first point or triangle that was selected
    
    // visualization stuff
    vtkSmartPointer<vtkPolyData> linePolyData_; // line polydata that is visualized as a visual response for selection of a face or a point
    vtkSmartPointer<vtkActor> lineActor_;
    vtkSmartPointer<vtkPolyDataMapper> lineMapper_;
};

#endif /* defined(CorrespondencePicker_H) */
