#include "Shape.h"

#include "vtkGeodesic.h"

#include <tgmath.h>

#include <vtkGlyph3D.h>
#include <vtkSphereSource.h>

// Constructor and Destructor
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

// Property Functions

double Shape::getEuclideanDistances(int start, std::vector<double> &distances) {
    
    double max = 0.0;
    distances.resize(polyData_->GetPoints()->GetNumberOfPoints());
    
    double reference[3], current[3];
    polyData_->GetPoints()->GetPoint(start, reference);
    
    // calculate all distances
    for(unsigned i = 0; i < polyData_->GetPoints()->GetNumberOfPoints(); ++i) {
        polyData_->GetPoints()->GetPoint(i, current);
        
        // keep track of distances and max distance
        distances[i] = std::sqrt(
                                 pow(reference[0] - current[0], 2) +
                                 pow(reference[1] - current[1], 2) +
                                 pow(reference[2] - current[2], 2)
                                 );
        if(distances[i] > max)
            max = distances[i];
        
    }
    
    return max;
}

// Visualization

void Shape::visualizeEuclidean(int start) {
    // random start if none was chosen
    if (start == -1)
        start = std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
    
    // initialize
    std::vector<double> distances;
    double max = getEuclideanDistances(start, distances);
    
    vtkSmartPointer<vtkLookupTable> colorLookupTable =
    vtkSmartPointer<vtkLookupTable>::New();
    colorLookupTable->SetTableRange(0, max);
    colorLookupTable->Build();
    
    // list of colors for vtk
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < this->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        polyData_->GetCellPoints(i, ids);
        
        // assign the distance of the closest point
        double dist = distances[ids->GetId(0)];
        if(dist > distances[ids->GetId(1)])
            dist = distances[ids->GetId(1)];
        if(dist > distances[ids->GetId(2)])
            dist = distances[ids->GetId(2)];
        
        double dcolor[3];
        colorLookupTable->GetColor(dist, dcolor);
        
        unsigned char color[3];
        for(unsigned int j = 0; j < 3; j++) {
            color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
        }
        
        colors->InsertNextTupleValue(color);
    }
    
    // update vtk
    polyData_->GetCellData()->SetScalars(colors);
    polyData_->Modified();
    
    polyDataNormals_->GetCellData()->SetScalars(colors);
    polyDataNormals_->Modified();
    
    renderer_->GetRenderWindow()->Render();
}

void Shape::visualizeVoronoiCells() {
    if (fps_->GetNumberOfIds() > 0) {
        visualizeVoronoiCells(fps_);
    } else {
        visualizeVoronoiCells(getFPS(10));
    }
}

void Shape::visualizeVoronoiCells(vtkSmartPointer<vtkIdList> points) {
    // initialize
    vtkSmartPointer<vtkIdList> voronoi = getVoronoiCells(points);
    double max = points->GetNumberOfIds();
    
    vtkSmartPointer<vtkLookupTable> colorLookupTable =
    vtkSmartPointer<vtkLookupTable>::New();
    colorLookupTable->SetTableRange(0, max - 1);
    colorLookupTable->SetNumberOfColors(12);
    colorLookupTable->SetScaleToLinear();
    //colorLookupTable->SetRampToLinear();
    colorLookupTable->SetSaturationRange(0.5, 1.0);
    colorLookupTable->SetHueRange(0, 1.0);
    colorLookupTable->SetValueRange(0.8, 1);
    colorLookupTable->Build();
    
    // list of colors for vtk
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < this->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        polyData_->GetCellPoints(i, ids);
        
        // assign the id that at least 2 points share, 0 otherwise
        double id;
        if(voronoi->GetId(ids->GetId(1)) == voronoi->GetId(ids->GetId(2))) {
            id = voronoi->GetId(ids->GetId(1));
        } else {
            id = voronoi->GetId(ids->GetId(0));
        }
        
        double dcolor[3];
        colorLookupTable->GetColor(id, dcolor);
        
        unsigned char color[3];
        for(unsigned int j = 0; j < 3; j++) {
            color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
        }
        
        colors->InsertNextTupleValue(color);
    }
    
    // update vtk
    polyData_->GetCellData()->SetScalars(colors);
    polyData_->Modified();
    
    polyDataNormals_->GetCellData()->SetScalars(colors);
    polyDataNormals_->Modified();
    
    renderer_->GetRenderWindow()->Render();
}

// Voronoi Cells

vtkSmartPointer<vtkIdList> Shape::getVoronoiCells(vtkSmartPointer<vtkIdList> points) {
    vtkGeodesic geodesic = vtkGeodesic(this, points);
    
    return geodesic.getVoronoiCells();
}

// FPS functions

// fps with given starting point
vtkSmartPointer<vtkIdList> Shape::getFPS(unsigned numberSamples, int start) {
    // random start if none was chosen
    if(start == -1)
        start = std::rand() % polyData_->GetPoints()->GetNumberOfPoints();
    
    // initialize id list with starting point
    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
    list->InsertNextId(start);
    
    //vtkGeodesic* geodesics = new vtkGeodesic(this, list);
    vtkGeodesic geodesics(this, list);
    
    for(int i = 1; i < numberSamples; i++) {
        
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
