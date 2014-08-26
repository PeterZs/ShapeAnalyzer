//
//  VoronoiCellSegmentation.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 26.08.14.
//
//

#include "VoronoiCellSegmentation.h"

vtkSmartPointer<vtkIdList> VoronoiCellSegmentation::getSegmentation() {
    vtkSmartPointer<vtkIdList> seeds = sampling_->getPoints();
    return metric_->getVoronoiCells(seeds);
}



/////////////////////////////////////////////////////////////////////////////////
//void Shape::visualizeVoronoiCells(vtkSmartPointer<vtkIdList> points) {
//    // initialize
//    vtkSmartPointer<vtkIdList> voronoi = getVoronoiCells(points);
//
//    vtkSmartPointer<vtkLookupTable> colorLookupTable =
//    vtkSmartPointer<vtkLookupTable>::New();
//    colorLookupTable->SetTableRange(0, points->GetNumberOfIds() - 1);
//    colorLookupTable->SetNumberOfColors(12);
//    colorLookupTable->SetScaleToLinear();
//    //colorLookupTable->SetRampToLinear();
//    colorLookupTable->SetSaturationRange(0.5, 1.0);
//    colorLookupTable->SetHueRange(0, 1.0);
//    colorLookupTable->SetValueRange(0.8, 1);
//    colorLookupTable->Build();
//    
//    // list of colors for vtk
//    vtkSmartPointer<vtkUnsignedCharArray> colors =
//    vtkSmartPointer<vtkUnsignedCharArray>::New();
//    colors->SetNumberOfComponents(3);
//    colors->SetName("Colors");
//    
//    for(int i = 0; i < this->getPolyData()->GetNumberOfCells(); i++) {
//        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
//        
//        // each cell has three id references to the corresponding points
//        polyData_->GetCellPoints(i, ids);
//        
//        // assign the id that at least 2 points share, 0 otherwise
//        double id;
//        if(voronoi->GetId(ids->GetId(1)) == voronoi->GetId(ids->GetId(2))) {
//            id = voronoi->GetId(ids->GetId(1));
//        } else {
//            id = voronoi->GetId(ids->GetId(0));
//        }
//        
//        double dcolor[3];
//        colorLookupTable->GetColor(id, dcolor);
//        
//        unsigned char color[3];
//        for(unsigned int j = 0; j < 3; j++) {
//            color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
//        }
//        
//        colors->InsertNextTupleValue(color);
//    }
//    
//    // update vtk
//    polyData_->GetCellData()->SetScalars(colors);
//    polyData_->Modified();
//    
//    polyDataNormals_->Update();
//    polyDataNormals_->Modified();
//    
//    renderer_->GetRenderWindow()->Render();
//}

//// Voronoi Cells
//
/////////////////////////////////////////////////////////////////////////////////
//vtkSmartPointer<vtkIdList> Shape::getVoronoiCells(vtkSmartPointer<vtkIdList> points) {
//    vtkGeodesic geodesic = vtkGeodesic(this, points);
//    
//    return geodesic.getVoronoiCells();
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// fps with given starting point
//vtkSmartPointer<vtkIdList> Shape::getFPS(unsigned numberSamples, int start) {
//    // random start if none was chosen
//    if(start == -1)
//        start = std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
//    
//    // initialize id list with starting point
//    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
//    list->InsertNextId(start);
//    
//    //vtkGeodesic* geodesics = new vtkGeodesic(this, list);
//    vtkGeodesic geodesics(this, list);
//    
//    for(int i = 1; i < numberSamples; i++) {
//        
//        list->InsertNextId(geodesics.findPointFurthestToAllSources());
//        
//        geodesics.changeSourcePoints(list);
//        
//    }
//    
//    return list;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//void Shape::setFPS(unsigned numberSamples, int start) {
//    fps_ = getFPS(numberSamples, start);
//    
//    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//    polyData_->GetPoints()->GetPoints(fps_, points);
//    
//    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
//    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
//    sphere->SetRadius(2.5);
//    polyData->SetPoints(points);
//    
//    // transform points into vertices
//    vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
//    
//    glyph3D->SetSourceConnection(sphere->GetOutputPort());
//    glyph3D->SetInputData(polyData);
//    glyph3D->Update();
//    
//    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper->SetInputConnection(glyph3D->GetOutputPort());
//    
//    fpsActor_->SetMapper(mapper);
//    fpsActor_->GetProperty()->SetColor(1, 0, 0);
//    
//    renderer_->AddActor(fpsActor_);
//    
//    
//    renderer_->GetRenderWindow()->Render();
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//void Shape::transformFPS(vtkLinearTransform *t) {
//    fpsActor_->SetUserTransform(t);
//}

