#include "Shape.h"

#include "vtkGeodesic.h"

#include <vtkGlyph3D.h>
#include <vtkSphereSource.h>

// Constructor
Shape::Shape(
             vtkIdType                      shapeId,
             vtkSmartPointer<vtkPolyData>   polyData,
             vtkSmartPointer<vtkPolyData>   polyDataNormals,
             vtkSmartPointer<vtkRenderer>   renderer
            ) :
             shapeId_(shapeId),
             polyData_(polyData),
             polyDataNormals_(polyDataNormals),
             renderer_(renderer)
{

    actor_ = vtkSmartPointer<vtkActor>::New();
    mapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper_->SetInputData(polyData_);
    

    actor_->SetMapper(mapper_);
    renderer_->AddActor(actor_);
    
    
    // set up box widget
    boxWidget_ = vtkSmartPointer<vtkBoxWidget>::New();
    boxWidget_->SetProp3D(actor_);
    boxWidget_->SetInteractor(renderer_->GetRenderWindow()->GetInteractor());
    boxWidget_->SetPlaceFactor(1.25);
    
    boxWidget_->PlaceWidget();
    
    fps_ = vtkSmartPointer<vtkIdList>::New();
    fpsActor_ = vtkSmartPointer<vtkActor>::New();
}

void Shape::remove() {
    renderer_->RemoveActor(fpsActor_);
    renderer_->RemoveActor(actor_);
    boxWidget_->SetInteractor(nullptr);
    boxWidget_->SetProp3D(nullptr);
}

// FPS functions

// fps with given starting point
vtkSmartPointer<vtkIdList> Shape::getFPS(unsigned numberSamples, int start) {
    if(start == -1)
        start =std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
    
    // initialize id list with starting point
    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
    list->InsertNextId(start);
    
    //vtkGeodesic* geodesics = new vtkGeodesic(this, list);
    vtkGeodesic geodesics(this, list);
    
    for(int i = 0; i < numberSamples; i++) {
        
        list->InsertNextId(geodesics.findPointFurthestToAllSources());
        
        geodesics.changeSourcePoints(list);
        
    }
    
    return list;
}

void Shape::setFPS(unsigned numberSamples, int start) {
    fps_ = getFPS(numberSamples, start);
    
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    polyData_->GetPoints()->GetPoints(fps_, points);
    
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(2.5);
    polydata->SetPoints(points);
    
    // transform points into vertices
    vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
#if VTK_MAJOR_VERSION <= 5
    glyph3D->SetSource(sphere->GetOutput());
    glyph3D->SetInput(polydata);
#else
    glyph3D->SetSourceConnection(sphere->GetOutputPort());
    glyph3D->SetInputData(polydata);
#endif
    glyph3D->Update();
    
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyph3D->GetOutputPort());
    
    fpsActor_->SetMapper(mapper);
    fpsActor_->GetProperty()->SetColor(1, 0, 0);
    
    renderer_->AddActor(fpsActor_);
  
    
    renderer_->GetRenderWindow()->Render();
}

void Shape::transformFPS(vtkLinearTransform *t) {
    fpsActor_->SetUserTransform(t);
}
