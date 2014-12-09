#include "PointCorrespondencePicker.h"

///////////////////////////////////////////////////////////////////////////////
void PointCorrespondencePicker::getCurrentSelectionPoint(Shape* shape, vtkIdType pointId, double point[3]) {
    // invalid input check
    if(shape == nullptr) {
        throw invalid_argument(string("Null pointer input 'shape' in ").append(__PRETTY_FUNCTION__));
    } else if (shape->getPolyData()->GetPoints()->GetNumberOfPoints() <= pointId) {
        throw invalid_argument("Source point (" + to_string(pointId) + ") larger than number of points (" + to_string(shape->getPolyData()->GetPoints()->GetNumberOfPoints()) + ") in " + __PRETTY_FUNCTION__);
    }
    
    shape->getPolyData()->GetPoint(pointId, point);
}


///////////////////////////////////////////////////////////////////////////////
void PointCorrespondencePicker::visualizeCurrentSelection(Shape* shape, vtkIdType pointId) {
    // invalid input check
    if(shape == nullptr) {
        throw invalid_argument(string("Null pointer input 'shape' in ").append(__PRETTY_FUNCTION__));
    } else if (shape->getPolyData()->GetPoints()->GetNumberOfPoints() <= pointId) {
        throw invalid_argument("Source point (" + to_string(pointId) + ") larger than number of points (" + to_string(shape->getPolyData()->GetPoints()->GetNumberOfPoints()) + ") in " + __PRETTY_FUNCTION__);
    }
    
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    double p[3];
    shape->getPolyData()->GetPoint(pointId, p);
    points->InsertNextPoint(p);
    
    polyData->SetPoints(points);
    
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputData(polyData);
    glyphFilter->Update();
    
    currentSelectionMapper_->SetInputData(glyphFilter->GetOutput());
    currentSelectionActor_->SetMapper(currentSelectionMapper_);
    currentSelectionActor_->GetProperty()->SetColor(0, 1, 1);
    currentSelectionActor_->GetProperty()->SetPointSize(5);
    currentSelectionActor_->GetProperty()->SetRepresentationToPoints();
    currentSelectionActor_->SetUserTransform(shape->getTransformation());
    renderer_->AddActor(currentSelectionActor_);
}


///////////////////////////////////////////////////////////////////////////////
Correspondence* PointCorrespondencePicker::createCorrespondence() {
    Correspondence* correspondence = new PointCorrespondence(renderer_, new PointCorrespondenceData(lastInsertCorrespondenceID_));
    correspondence->initialize();
    correspondence->addToRenderer();
    return correspondence;
}


