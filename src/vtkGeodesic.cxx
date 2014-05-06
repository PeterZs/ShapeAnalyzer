//
//  vtkGeodesic.cxx
//  ShapeAnalyzer
//
//  Created by Zorah on 27.04.14.
//
//

#include "vtkGeodesic.h"

using namespace geodesic;
using namespace std;

void vtkGeodesic::visualizeGeodesic(Shape *shape, QVTKWidget *qvtkWidget) {
    
    //initialize wrapper classes defined in vtkGeodesic.h instead of initializing vectors
    geodesicPoints points(shape->getPolyData());
    geodesicFaces faces(shape->getPolyData());
    
    Mesh mesh;
	mesh.initialize_mesh_data(points, faces);		//create internal mesh data structure including edges
    
	GeodesicAlgorithmExact algorithm(&mesh);	//create exact algorithm for the mesh
    
    // create random starting point
	unsigned source_vertex_index = std::rand() % shape->getPolyData()->GetNumberOfPoints();
    //unsigned source_vertex_index = 5;
    
	SurfacePoint source(&mesh.vertices()[source_vertex_index]);		//create source
	std::vector<geodesic::SurfacePoint> all_sources(1, source);
    
    algorithm.propagate(all_sources);	//cover the whole mesh
    
    // vector of all distances
    std::vector<double> distances;
    distances.resize(mesh.vertices().size());
    
    double max = 0;
    
    for(unsigned i = 0; i < mesh.vertices().size(); ++i) {
        SurfacePoint p(&mesh.vertices()[i]);
        
        double distance;
        algorithm.best_source(p, distance); //for a given surface point, find closets source and distance to this source
        
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
    
    for(int i = 0; i < shape->getPolyData()->GetNumberOfCells(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        shape->getPolyData()->GetCellPoints(i, ids);
        
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
    shape->getPolyDataNormals()->GetCellData()->SetScalars(colors);
    shape->getPolyDataNormals()->Modified();
    
    
    qvtkWidget->GetRenderWindow()->Render();
    
}

void vtkGeodesic::calculateGeodesic_gpu(Shape *shape) {
    //float d;
    //cudaMalloc(&d, sizeof(float));
}