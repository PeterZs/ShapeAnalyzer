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
#include <vtkUnsignedCharArray.h>

#include "geodesics/geodesic_algorithm_exact.h"

using namespace geodesic;


void vtkGeodesic::visualizeGeodesic(Shape *shape, QVTKWidget *qvtkWidget) {
    vtkSmartPointer<vtkPolyData> polydata = shape->getData();
    
    // extract poly data into vectors (because of the geodesic algorithm...)
    std::vector<double> points;
	std::vector<unsigned> faces;
    
//    bool success = geodesic::read_mesh_from_file("hedgehog_mesh.txt",points,faces);
//	if(!success)
//	{
//		std::cout << "something is wrong with the input file" << std::endl;
//	}
    
    vtkSmartPointer<vtkPoints> vtkPoints = polydata->GetPoints();
    
    // read all points and copy them into points
    // each points will be stored in 3 elements of the vector
    points.resize(3 * vtkPoints->GetNumberOfPoints());
    
    double *point;
    point = (double *) malloc(3 * sizeof(double));
    for(int i = 0; i < vtkPoints->GetNumberOfPoints(); i++) {
        
        //point = vtkPoints->GetData()->GetTuple(i);
        vtkPoints->GetPoint(i, point);
        points[i * 3] = point[0];
        points[i * 3 + 1] = point[1];
        points[i * 3 + 2] = point[2];
    }
    
    free(point);
    
    // read all faces and copy them into faces
    faces.resize(3 * polydata->GetNumberOfPolys());
    
    vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
    for(int i = 0; i < polydata->GetNumberOfPolys(); i++) {
        ids->Reset();
        
        // each cell has three id references to the corresponding points
        polydata->GetCellPoints(i, ids);
        faces[3 * i]        = ids->GetId(0);
        faces[3 * i + 1]    = ids->GetId(1);
        faces[3 * i + 2]    = ids->GetId(2);
    }
    
    Mesh mesh;
	mesh.initialize_mesh_data(points, faces);		//create internal mesh data structure including edges
    
	GeodesicAlgorithmExact algorithm(&mesh);	//create exact algorithm for the mesh
    
    // create random starting point
	unsigned source_vertex_index = std::rand() % vtkPoints->GetNumberOfPoints();
    //unsigned source_vertex_index = 5;
    
	SurfacePoint source(&mesh.vertices()[source_vertex_index]);		//create source
	std::vector<geodesic::SurfacePoint> all_sources(1, source);
    
    algorithm.propagate(all_sources);	//cover the whole mesh
    
    // vector of all distances
    std::vector<double> distances;
    distances.resize(mesh.vertices().size());
    
    double max = 0;
    
    for(unsigned i=0; i<mesh.vertices().size(); ++i)
    {
        SurfacePoint p(&mesh.vertices()[i]);
        
        double distance;
        unsigned best_source = algorithm.best_source(p,distance);		//for a given surface point, find closets source and distance to this source
        
        // keep track of distances and max distance
        distances[i] = distance;
        if(distance > max)
            max = distance;
        
    }
    
    
    // list of colors for vtk
    vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    
    for(int i = 0; i < polydata->GetNumberOfPolys(); i++) {
        vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
        
        // each cell has three id references to the corresponding points
        polydata->GetCellPoints(i, ids);
        
        // assign the distance of the closest point
        double dist = distances[ids->GetId(0)];
        if(dist > distances[ids->GetId(1)])
            dist = distances[ids->GetId(1)];
        if(dist > distances[ids->GetId(2)])
            dist = distances[ids->GetId(2)];
        
//        double dist = (distances[ids->GetId(0)] + distances[ids->GetId(1)] + distances[ids->GetId(2)] ) / 3;
        
        vtkGeodesic::COLOUR c = GetColour(dist,0,max);
        
        //unsigned char color[1] = {(dist / max) * 255};
        unsigned char color[3] = {c.r, c.g, c.b};
        
        colors->InsertNextTupleValue(color);
    }
    
    // update vtk
    polydata->GetCellData()->SetScalars(colors);
    
    //vtkSmartPointer<vtkPolyDataMapper> mapper =
    //vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = (vtkPolyDataMapper *) shape->getActor()->GetMapper();
    mapper->SetInputData(polydata);
    //mapper->SetColorModeToMapScalars();
    
    //shape->getActor()->SetMapper(mapper);
    
    
    qvtkWidget->GetRenderWindow()->Render();
    
}

/*
 Return a RGB colour value given a scalar v in the range [vmin,vmax]
 In this case each colour component ranges from 0 (no contribution) to
 1 (fully saturated), modifications for other ranges is trivial.
 The colour is clipped at the end of the scales if v is outside
 the range [vmin,vmax]
 */

vtkGeodesic::COLOUR vtkGeodesic::GetColour(double v,double vmin,double vmax)
{
    vtkGeodesic::COLOUR c = {255,255,255}; // white
    double dv;
    
    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    dv = vmax - vmin;
    
    if (v < (vmin + 0.25 * dv)) {
        c.r = 0;
        c.g = (int) 255 * (4 * (v - vmin) / dv);
    } else if (v < (vmin + 0.5 * dv)) {
        c.r = 0;
        c.b = (int) 255 * (1 + 4 * (vmin + 0.25 * dv - v) / dv);
    } else if (v < (vmin + 0.75 * dv)) {
        c.r = (int) 255 * (4 * (v - vmin - 0.5 * dv) / dv);
        c.b = 0;
    } else {
        c.g = (int) 255 * (1 + 4 * (vmin + 0.75 * dv - v) / dv);
        c.b = 0;
    }
    
    return(c);
}

void vtkGeodesic::calculateGeodesic_gpu(Shape *shape) {
    float d;
    //cudaMalloc(&d, sizeof(float));
}