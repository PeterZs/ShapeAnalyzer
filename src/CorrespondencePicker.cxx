//
//  CorrespondencePicker.cxx
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 25.04.14.
//
//

#include "CorrespondencePicker.h"


bool CorrespondencePicker::pick(Correspondence **correspondence, Shape *shape, vtkIdType selectionId) {
    selectedShape_ = shape;
    
    // visual response for picked face
    vtkSmartPointer<vtkPolyData> polyData = getSelectionPolyData(shape, selectionId);
    
    double point1[3];
    getSelectionPoint(shape, selectionId, point1);
    initializeLine(point1);
    
    createActor(selectionActor_, selectionMapper_, polyData, shape->getActor()->GetUserTransform());
    
    renderer_->AddActor(selectionActor_);
    
    // depending on whether there was a selection before or not
    if(waitForSelection_ == false) {
        // set source to current face or point and wait for selection of corresponding face or point
        id1_ = selectionId;
        
        actor1_ = createActor(polyData, shape->getActor()->GetUserTransform());
        
        shape1_ = shape;
        waitForSelection_ = true;
        return false;
    } else {
        // combine shape1 and current selection to a correspondence
        
        // if picked node is on the same shape as the source,
        // update and return
        if(shape1_ == shape) {
            id1_ = selectionId;
            actor1_ = createActor(polyData, shape->getActor()->GetUserTransform());
            return false;
        }
        
        // create correspondence
        *correspondence = createCorrespondence(renderer_, shape1_, shape, selectionId, actor1_, createActor(polyData, shape->getActor()->GetUserTransform()));
        renderer_->AddActor((*correspondence)->getLineActor());
        
        //remove green triangle and set flag to false again
        clearSelection();
        
        return true;
    }
}

void CorrespondencePicker::initializeLine(double point1[3]) {
    //initialize poly data and required data structures
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    linePolyData_ = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkCellArray> lineCells = vtkSmartPointer<vtkCellArray>::New();
    
    
    points->InsertNextPoint(point1);
    points->InsertNextPoint(point1);
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);
    lineCells->InsertNextCell(line);
    linePolyData_->SetPoints(points);
    linePolyData_->SetLines(lineCells);
    
    //create mapper and actor. mapper maps the poly data to the line actor
    lineMapper_->SetInputData(linePolyData_);
    
    vtkLinearTransform* t1 = selectedShape_->getActor()->GetUserTransform();
    if(t1 != nullptr) {
        linePolyData_->GetPoints()->SetPoint(0, t1->TransformPoint(point1));
        linePolyData_->GetPoints()->SetPoint(1, t1->TransformPoint(point1));
    }
    
    lineActor_->SetMapper(lineMapper_);
    lineActor_->GetProperty()->SetLineWidth(1);
    lineActor_->GetProperty()->SetColor(0, 1, 1);
    lineActor_->SetPickable(0);
    renderer_->AddActor(lineActor_);
}

vtkSmartPointer<vtkActor> CorrespondencePicker::createActor(vtkPolyData *polyData, vtkLinearTransform *t) {
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    
    createActor(actor, mapper, polyData, t);
    return actor;
}

void CorrespondencePicker::updateLine(int x, int y) {
    if(waitForSelection_) {
        vtkSmartPointer<vtkPointPlacer> pointPlacer = vtkSmartPointer<vtkPointPlacer>::New();
        double orientation[3];
        double point[3];
        double pos[2];
        pos[0] = x;
        pos[1] = y;
        pointPlacer->ComputeWorldPosition(renderer_, pos, point, orientation);
        linePolyData_->GetPoints()->SetPoint(0, point);
        linePolyData_->Modified();
        renderer_->GetRenderWindow()->Render();
    }
}

void CorrespondencePicker::clearSelection(Shape* shape) {
    if(shape == selectedShape_) {
        clearSelection();
    }
}

void CorrespondencePicker::clearSelection() {
    if(waitForSelection_ == true) {
        renderer_->RemoveActor(selectionActor_);
        renderer_->RemoveActor(lineActor_);
        renderer_->GetRenderWindow()->Render();
        waitForSelection_ = false;
    }
}

