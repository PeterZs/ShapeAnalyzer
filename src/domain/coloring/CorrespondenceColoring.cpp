//
//  CorrespondenceColoring.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 08.07.14.
//
//

#include "CorrespondenceColoring.h"

///////////////////////////////////////////////////////////////////////////////
CorrespondenceColoring::CorrespondenceColoring(HashMap<vtkActor*, Shape*>*               set,
                                               HashMap<PointCorrespondenceData*, bool>*  points,
                                               HashMap<FaceCorrespondenceData*, bool> *  faces,
                                               Shape* reference)
: shapes_(set), points_(points), faces_(faces), reference_(reference)
{
    if(reference_ == 0) {
        reference_ = shapes_->begin()->second;
    }
}


///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoring::showPointCorrespondences() {
    pointAttributes_.clear();
    
    // completely color reference shape
    CoordinateColoring cc = CoordinateColoring(reference_);
    cc.color();
    
    vtkIdType referenceId = reference_->getId();
    vtkSmartPointer<vtkUnsignedCharArray> referenceColors = cc.getColors();
    pointAttributes_.insert(
                        make_pair<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >(referenceId, referenceColors));
    
    // initialize color arrays for all shapes
    for (auto it = shapes_->begin(); it != shapes_->end(); it++) {
        if(it->second != reference_) {
            vtkSmartPointer<vtkUnsignedCharArray> array = vtkSmartPointer<vtkUnsignedCharArray>::New();
            array->SetNumberOfComponents(3);
            array->SetNumberOfTuples(it->second->getPolyData()->GetPoints()->GetNumberOfPoints());
            array->SetName("Colors");
            pointAttributes_.insert(make_pair<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >(it->second->getId(), array));
            
            // TODO sooo unschön, aber ich weiß nicht wie ich ihm vermittele, dass standardmäßig auf weiß gemappt werden soll
            double white[3] = {255, 255, 255};
            for(vtkIdType i = 0; i < it->second->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
                array->SetTuple(i, white);
            }
        }
    }
    
    // look up colors for non-reference shapes
    for(auto it = points_->begin(); it != points_->end(); it++) {
        vector<vtkIdType> shapes = it->first->getShapes();
        vector<vtkIdType> points = it->first->getCorrespondingIds();
        
        // id of correspondence on reference shape
        vtkIdType referenceCorrespondence = -1;
        
        // check if reference is in correspondence
        for(int i = 0; i < shapes.size(); i++) {
            if (shapes[i] == referenceId) {
                referenceCorrespondence = points[i];
                break;
            }
        }
        
        if(referenceCorrespondence != -1) {
            // look up color for each shape in correspondence
            for (int i = 0; i < shapes.size(); i++) {
                // dont color reference shape
                if (shapes[i] != referenceId) {
                if(pointAttributes_.count(shapes[i]) != 0) {
                    vtkSmartPointer<vtkUnsignedCharArray> colors = pointAttributes_.find(shapes[i])->second;
                    // color of point is the same as the corresponding point on the reference shape has
                    double color[3];
                    referenceColors->GetTuple(referenceCorrespondence, color);
                    colors->SetTuple(points[i], color);
                }
                }
            }
        }
    }
    
    // color all shapes
    for (auto it = shapes_->begin(); it != shapes_->end(); it++) {
        if(it->second != reference_) {
            vtkSmartPointer<vtkUnsignedCharArray> colors = pointAttributes_.find(it->second->getId())->second;
            it->second->getPolyData()->GetPointData()->SetScalars(colors);
            it->second->getMapper()->SetScalarModeToUsePointData();
        }
    }
    
    // rerender
    reference_->getRenderer()->GetRenderWindow()->Render();
}

///////////////////////////////////////////////////////////////////////////////
void CorrespondenceColoring::showFaceCorrespondences() {
    faceAttributes_.clear();
    
    // completely color reference shape
    FaceCoordinateColoring cc = FaceCoordinateColoring(reference_);
    cc.color();
    
    vtkIdType referenceId = reference_->getId();
    vtkSmartPointer<vtkUnsignedCharArray> referenceColors = cc.getColors();
    faceAttributes_.insert(
                            make_pair<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >(referenceId, referenceColors));
    
    // initialize color arrays for all shapes
    for (auto it = shapes_->begin(); it != shapes_->end(); it++) {
        if(it->second != reference_) {
            vtkSmartPointer<vtkUnsignedCharArray> array = vtkSmartPointer<vtkUnsignedCharArray>::New();
            array->SetNumberOfComponents(3);
            array->SetNumberOfTuples(it->second->getPolyData()->GetNumberOfCells());
            array->SetName("Colors");
            faceAttributes_.insert(make_pair<vtkIdType, vtkSmartPointer<vtkUnsignedCharArray> >(it->second->getId(), array));
            
            // TODO sooo unschön, aber ich weiß nicht wie ich ihm vermittele, dass standardmäßig auf weiß gemappt werden soll
            double white[3] = {255, 255, 255};
            for(vtkIdType i = 0; i < it->second->getPolyData()->GetNumberOfCells(); i++) {
                array->SetTuple(i, white);
            }
        }
    }
    
    // look up colors for non-reference shapes
    for(auto it = faces_->begin(); it != faces_->end(); it++) {
        vector<vtkIdType> shapes = it->first->getShapes();
        vector<vtkIdType> points = it->first->getCorrespondingIds();
        
        // id of correspondence on reference shape
        vtkIdType referenceCorrespondence = -1;
        
        // check if reference is in correspondence
        for(int i = 0; i < shapes.size(); i++) {
            if (shapes[i] == referenceId) {
                referenceCorrespondence = points[i];
                break;
            }
        }
        
        if(referenceCorrespondence != -1) {
            // look up color for each shape in correspondence
            for (int i = 0; i < shapes.size(); i++) {
                // dont color reference shape
                if (shapes[i] != referenceId) {
                    if(faceAttributes_.count(shapes[i]) != 0) {
                        vtkSmartPointer<vtkUnsignedCharArray> colors = faceAttributes_.find(shapes[i])->second;
                        // color of point is the same as the corresponding point on the reference shape has
                        double color[3];
                        referenceColors->GetTuple(referenceCorrespondence, color);
                        colors->SetTuple(points[i], color);
                    }
                }
            }
        }
    }
    
    // color all shapes
    for (auto it = shapes_->begin(); it != shapes_->end(); it++) {
        if(it->second != reference_) {
            vtkSmartPointer<vtkUnsignedCharArray> colors = faceAttributes_.find(it->second->getId())->second;
            it->second->getPolyData()->GetCellData()->SetScalars(colors);
            it->second->getMapper()->SetScalarModeToUseCellData();
        }
    }
    
    // rerender
    reference_->getRenderer()->GetRenderWindow()->Render();
}