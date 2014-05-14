//
//  vtkGeodesic.cxx
//  ShapeAnalyzer
//
//  Created by Zorah on 27.04.14.
//
//

#include "vtkGeodesic.h"

#include <math.h>

#include <vtkCellData.h>
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>

using namespace geodesic;
using namespace std;

//Always provide a destructor that deletes all objects that are created somewhere somewhen in the class.
//Every new requires a delete.
vtkGeodesic::~vtkGeodesic() {
    delete points_;
    delete faces_;
    delete algorithm_;
}

vtkGeodesic::vtkGeodesic(Shape *shape) : shape_(shape) {
    // create random starting point
	source_ = std::rand() % shape->getPolyData()->GetPoints()->GetNumberOfPoints();
    

    initialize();
}

vtkGeodesic::vtkGeodesic(Shape *shape, unsigned s) : shape_(shape), source_(s) {

    initialize();
}

void vtkGeodesic::initialize() {
    
    //initialize wrapper classes defined in vtkGeodesic.h instead of initializing vectors
    points_ = new geodesicPoints(shape_->getPolyData());
    faces_ = new geodesicFaces(shape_->getPolyData());
    
	mesh_.initialize_mesh_data(*points_, *faces_);		//create internal mesh data structure including edges
    
    algorithm_ = new GeodesicAlgorithmExact(&mesh_);
    
    SurfacePoint source(&mesh_.vertices()[source_]);		//create source
	std::vector<geodesic::SurfacePoint> all_sources(1, source);
    
    algorithm_->propagate(all_sources);	//cover the whole mesh
    
}

void vtkGeodesic::changeSourcePoint(unsigned s) {
    source_ = s;
    
    SurfacePoint source(&mesh_.vertices()[source_]);		//create source
	std::vector<geodesic::SurfacePoint> all_sources(1, source);
    
    algorithm_->propagate(all_sources);	//cover the whole mesh
}


void vtkGeodesic::visualizeGeodesic(QVTKWidget *qvtkWidget) {
    
    // vector of all distances
    std::vector<double> distances;
    distances.resize(mesh_.vertices().size());
    
    double max = 0;
    
    for(unsigned i = 0; i < mesh_.vertices().size(); ++i) {
        SurfacePoint p(&mesh_.vertices()[i]);
        
        double distance;
        algorithm_->best_source(p, distance); //for a given surface point, find closets source and distance to this source
        
        // keep track of distances and max distance
        distances[i] = distance;
        if(distance > max)
            max = distance;
        
    }
    
    vtkSmartPointer<vtkLookupTable> colorLookupTable =
    vtkSmartPointer<vtkLookupTable>::New();
    colorLookupTable->SetTableRange(0, max);
    colorLookupTable->Build();
    
    // list of colors for vtk
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < shape_->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        shape_->getPolyData()->GetCellPoints(i, ids);
        
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
    shape_->getPolyData()->GetCellData()->SetScalars(colors);
    shape_->getPolyData()->Modified();
    
    shape_->getPolyDataNormals()->GetCellData()->SetScalars(colors);
    shape_->getPolyDataNormals()->Modified();
    
    qvtkWidget->GetRenderWindow()->Render();
}

void vtkGeodesic::calculateGeodesic_gpu() {
    //float d;
    //cudaMalloc(&d, sizeof(float));
}