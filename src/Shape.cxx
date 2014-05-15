#include "Shape.h"

#include "vtkGeodesic.h"

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
}

void Shape::remove() {
    renderer_->RemoveActor(actor_);
    boxWidget_->SetInteractor(nullptr);
    boxWidget_->SetProp3D(nullptr);
}

// fps with random starting point
vtkSmartPointer<vtkIdList> Shape::getFPS(unsigned numberSamples) {
    return getFPS(numberSamples, std::rand() % polyData_->GetPoints()->GetNumberOfPoints());
}

// fps with given starting point
vtkSmartPointer<vtkIdList> Shape::getFPS(unsigned numberSamples, unsigned start) {
    // initialize id list with starting point
    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
    list->InsertNextId(start);
    
    vtkGeodesic* geodesics = new vtkGeodesic(this, start);
    
    for(int i = 0; i < numberSamples; i++) {
        
    }
    
    delete geodesics;
    
    return list;
}
