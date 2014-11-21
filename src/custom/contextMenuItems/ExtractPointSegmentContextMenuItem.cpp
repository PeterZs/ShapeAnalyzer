//
//  ExtractPointSegmentContextMenuItem.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#include "ExtractPointSegmentContextMenuItem.h"


void ExtractPointSegmentContextMenuItem::onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent) {
    
    if(pointId == -1 || shape->getColoring() == nullptr || shape->getColoring()->type != Shape::Coloring::Type::PointSegmentation) {
        QMessageBox::warning(parent, "Error", "No valid point segment selected.\nDoes shape \"" + QString(shape->getName().c_str()) + "\" have an active point segmentation? Eventually a point segmentation has to be set first.");
        return;
    }
    
    
    vtkIdType segmentId = shape->getColoring()->values->GetTuple(pointId)[0];
    
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    
    // remeber already added points
    HashMap<vtkIdType, vtkIdType> pointIds;
    for(vtkIdType i = 0; i < shape->getPolyData()->GetNumberOfCells(); i++) {
        vtkTriangle* face = (vtkTriangle*) shape->getPolyData()->GetCell(i);
        
        for(vtkIdType j = 0; j < 3; j++) {
            //check if current cell is contained in segment. I.e. on of the points of current cell must be contained in segment.
            
            if(shape->getColoring()->values->GetTuple(face->GetPointId(j))[0] == segmentId) {
                vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
                
                // copy all the points of face into points. Create new triangle that corresponds to currentFace
                for(vtkIdType k = 0; k < 3; k++) {
                    if(pointIds.containsKey(face->GetPointId(k))) {
                        triangle->GetPointIds()->SetId(k, pointIds[face->GetPointId(k)]);
                    } else {
                        double x[3];
                        shape->getPolyData()->GetPoint(face->GetPointId(k), x);
                        vtkIdType pointId = points->InsertNextPoint(x); // new point id
                        triangle->GetPointIds()->SetId(k, pointId);
                        pointIds.insert(face->GetPointId(k), pointId);
                    }
                }
                
                
                polys->InsertNextCell(triangle);
                break;
            }
        }
    }
    
    polyData->SetPoints(points);
    polyData->SetPolys(polys);
    
    string name = shape->getName();
    name.append(":0");
    dynamic_cast<ShapeAnalyzerInterface*>(parent)->addShape(name, polyData);
}
