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

#include <list>
#include <algorithm>

#include "../domain/Shape.h"
#include "../domain/correspondences/Correspondence.h"
#include "../domain/correspondences/CorrespondenceData.h"

//Class responsible for creating a correspondence
class CorrespondencePicker {
public:
    virtual ~CorrespondencePicker() {
        clearSelection();
    }
    
    //add face (or vertex) ID + shape pair to correspondence. Returns true if id+shape pair has successfully been added (i.e. if shape has not been added twice).
    int add(Shape* shape, vtkIdType selectionId);
    
    //returns true in case at least two shapes have been added to correspondence and returns new correspondence via argument. Otherwise returns false.
    bool pick(Correspondence** correspondence);
    
    void updateMouseLine(int x, int y);
    void clearSelection();

protected:
    //protected contructor: abstract class
    CorrespondencePicker(vtkRenderer* renderer) : renderer_(renderer), counter_(0) {
        currentSelectionActor_ = vtkSmartPointer<vtkActor>::New();
        currentSelectionMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
        
        mouseLineActor_ = vtkSmartPointer<vtkActor>::New();
        mouseLineMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    
    //returns point coordinates of current selection (center of selected triangle in case of FaceCorrespondence) to draw line between this point and mouse coordinates
    virtual void getCurrentSelectionPoint(Shape* shape, vtkIdType, double point[3]) = 0;

    virtual void visualizeCurrentSelection(Shape* shape, vtkIdType) = 0;

    virtual Correspondence* createCorrespondence() = 0;
    
    vtkSmartPointer<vtkActor> currentSelectionActor_; // actor responsible for visualizing selected vertex or selected face on selected shape
    vtkSmartPointer<vtkPolyDataMapper> currentSelectionMapper_;
    
    vtkRenderer* renderer_;
    
    CorrespondenceData* data_;
private:
    void visualizeMouseLine(Shape* shape, double point[3]);
    
    void clearRenderer();
    void clearPicker();
    
    Correspondence* correspondence_;
    int counter_;
    

    // visualization stuff
    
    // moving line between last selected point and mouse coordinates
    vtkSmartPointer<vtkActor> mouseLineActor_;
    vtkSmartPointer<vtkPolyDataMapper> mouseLineMapper_;
};

#endif /* defined(CorrespondencePicker_H) */
