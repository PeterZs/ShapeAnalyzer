//
//  CorrespondencePicker.cxx
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 25.04.14.
//
//

#include "CorrespondencePicker.h"

///////////////////////////////////////////////////////////////////////////////
int CorrespondencePicker::addShape(Shape* shape, vtkIdType selectionId) throw(std::invalid_argument) {
    // invalid input check
    if(shape == nullptr) {
        throw invalid_argument(string("Null pointer input 'shape' in ").append(__PRETTY_FUNCTION__));
    }
    if(counter_ == 0) {
        correspondence_ = createCorrespondence();
    }
    
    // visual response for picked face or vertex
    double source[3];
    
    //get point that is the source of the moving line
    getCurrentSelectionPoint(shape, selectionId, source);
    
    //visualize mouse line drawn from source to mouse coordinates
    visualizeMouseLine(shape, source);
    
    //visualize selected vertex or face
    visualizeCurrentSelection(shape, selectionId);

    int result = correspondence_->addShape(shape, selectionId);
    
    if(result == 1) { // success in adding the selection
        counter_++;
        
        return result;
    }
    if(result == -1) { // clicked shape is already part of correspondence and not equal to the last added
        // reset selection
        counter_ = 1;
        correspondence_->removeFromRenderer();
        delete correspondence_;
        correspondence_ = createCorrespondence();
        correspondence_->addShape(shape, selectionId);
        return result;
    }
    
    return result;
}


///////////////////////////////////////////////////////////////////////////////
// stores the current selection in the given pointer (if the selection is a
// valid correspondence) and resets the picker
bool CorrespondencePicker::pick(Correspondence** correspondence) {
    if(counter_ >= 2) {
        renderer_->RemoveActor(currentSelectionActor_);
        renderer_->RemoveActor(mouseLineActor_);
        renderer_->GetRenderWindow()->Render();
        
        *correspondence = correspondence_;
        //reset counter
        counter_ = 0;
        return true;
    }

    clearSelection();
    return false;
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondencePicker::visualizeMouseLine(Shape* shape, double point[3]) {
    //initialize poly data and required data structures
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> mouseLinePolyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkCellArray> lineCells = vtkSmartPointer<vtkCellArray>::New();
    
    
    points->InsertNextPoint(point);
    points->InsertNextPoint(point);
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);
    lineCells->InsertNextCell(line);
    mouseLinePolyData->SetPoints(points);
    mouseLinePolyData->SetLines(lineCells);
    
    //create mapper and actor. mapper maps the poly data to the line actor
    mouseLineMapper_->SetInputData(mouseLinePolyData);
    
    vtkLinearTransform* t1 = shape->getActor()->GetUserTransform();
    if(t1 != nullptr) {
        mouseLinePolyData->GetPoints()->SetPoint(0, t1->TransformPoint(point));
        mouseLinePolyData->GetPoints()->SetPoint(1, t1->TransformPoint(point));
    }
    
    mouseLineActor_->SetMapper(mouseLineMapper_);
    mouseLineActor_->GetProperty()->SetLineWidth(1);
    mouseLineActor_->GetProperty()->SetColor(0, 1, 1);
    mouseLineActor_->SetPickable(0);
    renderer_->AddActor(mouseLineActor_);
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondencePicker::updateMouseLine(int x, int y) {
    if(counter_ > 0) {
        vtkSmartPointer<vtkPointPlacer> pointPlacer = vtkSmartPointer<vtkPointPlacer>::New();
        double orientation[3];
        double point[3];
        double pos[2];
        pos[0] = x;
        pos[1] = y;
        pointPlacer->ComputeWorldPosition(renderer_, pos, point, orientation);
        
        mouseLineMapper_->GetInput()->GetPoints()->SetPoint(0, point);
        mouseLineMapper_->GetInput()->Modified();
        renderer_->GetRenderWindow()->Render();
    }
}


///////////////////////////////////////////////////////////////////////////////
// Resets information and deletes actors from renderer
void CorrespondencePicker::clearSelection() {
    if(counter_ > 0) {
        renderer_->RemoveActor(currentSelectionActor_);
        renderer_->RemoveActor(mouseLineActor_);
        correspondence_->removeFromRenderer();
        renderer_->GetRenderWindow()->Render();
        delete correspondence_->getData();
        delete correspondence_;
        counter_ = 0;
    }
}


