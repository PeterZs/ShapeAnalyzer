//
//  GeodesicMetric.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "GeodesicMetric.h"


///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// initialization of the internal data structure and an precomputation-free
// algorithm
void GeodesicMetric::initialize(Shape *shape) {
    Metric::initialize(shape);
    
    points_ = new geodesicPoints(shape_->getPolyData());
    faces_ = new geodesicFaces(shape_->getPolyData());
    
    try {
        mesh_.initialize_mesh_data(*points_, *faces_);		//create internal mesh data structure including edges
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
    }
    algorithm_ = new GeodesicAlgorithmExact(&mesh_);
}

///////////////////////////////////////////////////////////////////////////////
GeodesicMetric::~GeodesicMetric() {
    delete algorithm_;
    delete points_;
    delete faces_;
}


///////////////////////////////////////////////////////////////////////////////
// Implemented Metric Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Returns a vector with all distances on the shape to the source s ordered
// by their id
// Notice that this will remove the current sources and precomputed information
void GeodesicMetric::getAllDistances(ScalarPointAttribute& distances, vtkIdType s) {
    // initialize algorithm for this source
    SurfacePoint source(&mesh_.vertices()[s]);
    vector<SurfacePoint> all_sources(1, source);
    try {
        algorithm_->propagate(all_sources);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
    }
    
    for(vtkIdType i = 0; i< mesh_.vertices().size(); ++i) {
        geodesic::SurfacePoint p(&mesh_.vertices()[i]);
        
        double distance;
        try {
            algorithm_->best_source(p, distance); //for a given surface point, find closets source and distance to this source
        } catch (geodesic_error& e) {
            cout << e.what() << '\n';
        }
        
        distances.getScalars()->SetValue(i, distance);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Returns distance between a and b
// Notice that this will remove the current sources and precomputed information
// about these
double GeodesicMetric::getDistance(vtkIdType a, vtkIdType b) {
    SurfacePoint source(&mesh_.vertices()[a]);
    SurfacePoint target(&mesh_.vertices()[b]);
    
    vector<SurfacePoint> path;
    
    try {
        algorithm_->geodesic(source, target, path);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
        return 0.0;
    }
    
    return length(path);
}


///////////////////////////////////////////////////////////////////////////////
// Returns the point that is farthest away from all points in sources
// Notice that this will remove the current sources and precomputed information
// about these if the input list is not the same as the current source list
vtkIdType GeodesicMetric::getFarthestPoint(vtkSmartPointer<vtkIdList> sources) {
    vector<SurfacePoint> all_sources;
    all_sources.resize(sources->GetNumberOfIds());
    
    // create SurfacePoints for all ids
    for(int i = 0; i < sources->GetNumberOfIds(); i++) {
        SurfacePoint source(&mesh_.vertices()[sources->GetId(i)]); //create source
        all_sources[i] = source;
    }
    
    // recomputation of geodesics
    try {
        algorithm_->propagate(all_sources);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
    }
    
    double maxDist = 0.0;
    
    vtkIdType id = 0;
    
    // iterate over all points
    for(int i = 0; i < mesh_.vertices().size(); i++) {
        SurfacePoint target(&mesh_.vertices()[i]); //create source
        
        // calculate shortest distance to some source
        double dist;
        try {
            algorithm_->best_source(target, dist);
        } catch (geodesic_error& e) {
            cout << e.what() << '\n';
            return 0;
        }
        
        // check if shortest distance is the greatest so far
        if(dist > maxDist) {
            maxDist = dist;
            id = i;
        }
        
    }
    
    return id;
}

vtkSmartPointer<vtkIdList> GeodesicMetric::getVoronoiCells(vtkSmartPointer<vtkIdList> seeds) {
    vtkSmartPointer<vtkIdList> voronoiCells = vtkSmartPointer<vtkIdList>::New();
    voronoiCells->SetNumberOfIds(shape_->getPolyData()->GetPoints()->GetNumberOfPoints());
    
    
    vector<SurfacePoint> all_sources;
    all_sources.resize(seeds->GetNumberOfIds());
    
    // create SurfacePoints for all ids
    for(int i = 0; i < seeds->GetNumberOfIds(); i++) {
        SurfacePoint source(&mesh_.vertices()[seeds->GetId(i)]); //create source
        all_sources[i] = source;
    }
    
    // recomputation of geodesics
    try {
        algorithm_->propagate(all_sources);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
    }
    
    double dist;
    
    for(vtkIdType i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        SurfacePoint p(&mesh_.vertices()[i]);
        voronoiCells->SetId(i, algorithm_->best_source(p, dist));
    }
    
    return voronoiCells;
}



