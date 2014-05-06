//
//  CorrespondencePicker.cxx
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 25.04.14.
//
//

#include "CorrespondencePicker.h"

//pick correspondence: first call returns false always. second call return true iff two different shapes have been selected in two subsequent calls. Correspondence is created at address *correspondence given as an argument.
bool CorrespondencePicker::pick(Correspondence **correspondence, Shape *shape, vtkIdType cellId) {
    selectedShape_ = shape;
    
    // visual response for picked face
    vtkSmartPointer<vtkUnstructuredGrid> selection = getSelectionGrid(shape, cellId);

    selectedFaceMapper_->SetInputData(selection);
    selectedFaceActor_->SetMapper(selectedFaceMapper_);
    selectedFaceActor_->GetProperty()->EdgeVisibilityOn();
    selectedFaceActor_->GetProperty()->SetEdgeColor(1, 1, 0);
    selectedFaceActor_->GetProperty()->SetLineWidth(3);
    selectedFaceActor_->SetUserTransform(shape->getActor()->GetUserTransform());
    
    renderer_->AddActor(selectedFaceActor_);
    
    //initialize poly data and required data structures
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    linePolyData_ = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    vtkSmartPointer<vtkCellArray> lineCells = vtkSmartPointer<vtkCellArray>::New();
    
    // polyData for moving line
    double p1[3];
    double p2[3];
    double p3[3];
    double point1[3];
    
    vtkSmartPointer<vtkTriangle> face1 = vtkTriangle::SafeDownCast(selection->GetCell(0));
    
    face1->GetPoints()->GetPoint(0, p1);
    face1->GetPoints()->GetPoint(1, p2);
    face1->GetPoints()->GetPoint(2, p3);
    vtkTriangle::TriangleCenter(p1, p2, p3, point1);
    
    points->InsertNextPoint(point1);
    points->InsertNextPoint(point1);
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);
    lineCells->InsertNextCell(line);
    linePolyData_->SetPoints(points);
    linePolyData_->SetLines(lineCells);
    
    //create mapper and actor. mapper maps the poly data to the line actor
    lineMapper_->SetInputData(linePolyData_);
    
    vtkLinearTransform* t1 = shape->getActor()->GetUserTransform();
    if(t1 != nullptr) {
        linePolyData_->GetPoints()->SetPoint(0, t1->TransformPoint(point1));
        linePolyData_->GetPoints()->SetPoint(1, t1->TransformPoint(point1));
    }
    
    lineActor_->SetMapper(lineMapper_);
    lineActor_->GetProperty()->SetLineWidth(1);
    lineActor_->GetProperty()->SetColor(1, 1, 0);
    lineActor_->SetPickable(0);
    renderer_->AddActor(lineActor_);

    // depending on whether there was a selection before or not
    if(waitForSelection_ == false) {
        // set source to current triangle and wait for selection of corresponding triangle
        face1_ = vtkTriangle::SafeDownCast(selection->GetCell(0));
        face1Actor_ = createFaceActorFromGrid(selection, shape->getActor()->GetUserTransform());
        shape1_ = shape;
        waitForSelection_ = true;
        return false;
    } else {
        // combine shape1 and current pick to a correspondence
        
        // if picked node is on the same shape as the source,
        // update and return
        if(shape1_ == shape) {
            face1_ = vtkTriangle::SafeDownCast(selection->GetCell(0));
            face1Actor_ = createFaceActorFromGrid(selection, shape->getActor()->GetUserTransform());
            return false;
        }
        
        // create correspondence
        *correspondence = new Correspondence(shape1_, shape, face1_, vtkTriangle::SafeDownCast(selection->GetCell(0)), face1Actor_, createFaceActorFromGrid(selection, shape->getActor()->GetUserTransform()));
        renderer_->AddActor((*correspondence)->getActor());
        //remove green triangle and set flag to false again
        waitForSelection_ = false;
        renderer_->RemoveActor(selectedFaceActor_);
        renderer_->RemoveActor(lineActor_);
        return true;
    }
}

void CorrespondencePicker::mouseMoveHandler(int x, int y) {
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

vtkSmartPointer<vtkActor> CorrespondencePicker::createFaceActorFromGrid(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkLinearTransform* t) {
    vtkSmartPointer<vtkDataSetMapper> triangleMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    triangleMapper->SetInputData(grid);
    vtkSmartPointer<vtkActor> triangleActor = vtkSmartPointer<vtkActor>::New();
    triangleActor->SetMapper(triangleMapper);
    triangleActor->GetProperty()->EdgeVisibilityOn();
    triangleActor->GetProperty()->SetEdgeColor(1, 0, 0);
    triangleActor->GetProperty()->SetLineWidth(3);
    triangleActor->SetUserTransform(t);
    return triangleActor;
}

vtkSmartPointer<vtkUnstructuredGrid> CorrespondencePicker::getSelectionGrid(Shape *shape, vtkIdType cellId) {
    // create new Id object for this pick
    vtkSmartPointer<vtkIdTypeArray> ids =
    vtkSmartPointer<vtkIdTypeArray>::New();
    ids->SetNumberOfComponents(1);
    ids->InsertNextValue(cellId);
    
    // mark picked node as selected
    vtkSmartPointer<vtkSelectionNode> selectionNode =
    vtkSmartPointer<vtkSelectionNode>::New();
    selectionNode->SetFieldType(vtkSelectionNode::CELL);
    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    selectionNode->SetSelectionList(ids);
    
    vtkSmartPointer<vtkSelection> selection =
    vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(selectionNode);
    vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
    extractSelection->SetInputData(0, shape->getPolyData());
    extractSelection->SetInputData(1, selection);
    extractSelection->Update();
    
    // In selection
    vtkSmartPointer<vtkUnstructuredGrid> selectionGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    selectionGrid->ShallowCopy(extractSelection->GetOutput());
    
    assert(selectionGrid->GetNumberOfCells() == 1);

    assert(selectionGrid->GetCellType(0) == VTK_TRIANGLE);
    
    return selectionGrid;
}

void CorrespondencePicker::clearSelection(Shape* shape) {
    if(shape == selectedShape_) {
        clearSelection();
    }
}

void CorrespondencePicker::clearSelection() {
    if(waitForSelection_ == true) {
        renderer_->RemoveActor(selectedFaceActor_);
        renderer_->RemoveActor(lineActor_);
        renderer_->GetRenderWindow()->Render();
        waitForSelection_ = false;
    }
}

