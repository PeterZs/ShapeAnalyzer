//
//  Geodesic.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "Geodesic.h"


///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// initialization of the internal data structure and an precomputation-free
// algorithm
Geodesic::Geodesic(Shape* shape) {
    identifier_ = "Geodesic";
    shape_ = shape;
    
    points_ = new geodesicPoints(shape_->getPolyData());
    faces_ = new geodesicFaces(shape_->getPolyData());
    
	mesh_.initialize_mesh_data(*points_, *faces_);		//create internal mesh data structure including edges
    
    algorithm_ = new GeodesicAlgorithmExact(&mesh_);
}

///////////////////////////////////////////////////////////////////////////////
Geodesic::~Geodesic() {
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
vector<double> Geodesic::getAllDistances(unsigned int s) {
    // initialize result vector
    vector<double> distances;
    distances.resize(shape_->getPolyData()->GetPoints()->GetNumberOfPoints());
    
    // initialize algorithm for this source
    SurfacePoint source(&mesh_.vertices()[s]);
    vector<geodesic::SurfacePoint> all_sources(1,source);
    algorithm_->propagate(all_sources);
    
    for(unsigned i = 0; i< mesh_.vertices().size(); ++i)
    {
        geodesic::SurfacePoint p(&mesh_.vertices()[i]);
        
        double distance;
        algorithm_->best_source(p,distance);		//for a given surface point, find closets source and distance to this source
        
        distances[i] = distance;
    }
    
    return distances;
}

///////////////////////////////////////////////////////////////////////////////
// Returns distance between a and b
// Notice that this will remove the current sources and precomputed information
// about these
double Geodesic::getDistance(unsigned a, unsigned b) {
    SurfacePoint source(&mesh_.vertices()[a]);
    SurfacePoint target(&mesh_.vertices()[b]);
    
    vector<SurfacePoint> path;
    
    sourceList_ = vtkSmartPointer<vtkIdList>::New();
    sources_ = vector<SurfacePoint>();
    
    algorithm_->geodesic(source, target, path);
    
    return calculateLengthOfPath(path);
}


///////////////////////////////////////////////////////////////////////////////
// Returns the point furthest to all points in sources
// Notice that this will remove the current sources and precomputed information
// about these if the input list is not the same as the current source list
unsigned Geodesic::getPointFurthestToAllSources(vtkSmartPointer<vtkIdList> sources) {
    double distance = 0;
    unsigned id;
    
    
    unsigned number = sourceList_->GetNumberOfIds();
    sourceList_->IntersectWith(sources);
    
    // sources are not the same as the given list, recompute
    if (sourceList_->GetNumberOfIds() < number || sources->GetNumberOfIds() != number) {
        // recompute geodesics
        changeSourcePoints(sources);
    }
    
    // iterate over all points
    for(int i = 0; i < mesh_.vertices().size(); i++) {
        SurfacePoint target(&mesh_.vertices()[i]); //create source
        
        // calculate shortest distance to some source
        double dist;
        algorithm_->best_source(target, dist);
        
        // check if shortest distance is the greatest so far
        if(dist > distance) {
            distance = dist;
            id = i;
        }
        
    }
    
    return id;
}


///////////////////////////////////////////////////////////////////////////////
// Geodesic Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// changes the source to points with id s
// other already computed distances will be lost
void Geodesic::changeSourcePoint(unsigned s) {
    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
    list->InsertNextId(s);
    
    changeSourcePoints(list);
}


///////////////////////////////////////////////////////////////////////////////
// changes the source to points with ids in s
// other already computed distances will be lost
void Geodesic::changeSourcePoints(vtkSmartPointer<vtkIdList> s) {
    sourceList_ = s;
    sources_.resize(s->GetNumberOfIds());
    
    // create SurfacePoints for all ids
    for(int i = 0; i < s->GetNumberOfIds(); i++) {
        SurfacePoint source(&mesh_.vertices()[s->GetId(i)]); //create source
        sources_[i] = source;
    }
    
    // recomputation of geodesics
    algorithm_->propagate(sources_);
}


///////////////////////////////////////////////////////////////////////////////
// returns id of points furthest to all sources
unsigned Geodesic::findPointFurthestToAllSources() {
    
    unsigned id = 0;
    double distance = 0.0;
    
    // iterate over all points
    for(int i = 0; i < mesh_.vertices().size(); i++) {
        SurfacePoint target(&mesh_.vertices()[i]); //create source
        
        // calculate shortest distance to some source
        double dist;
        algorithm_->best_source(target, dist);
        
        // check if shortest distance is the greatest so far
        if(dist > distance) {
            distance = dist;
            id = i;
        }
        
    }
    
    return id;
}

///////////////////////////////////////////////////////////////////////////////
// Geodesic Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// calculates the length of the path given by the SurfacePoints in path
double Geodesic::calculateLengthOfPath(vector<SurfacePoint> path) {
    double length = 0;
    
    // iterate over all points but the last
    for(int i = 0; i < path.size() - 1; i++) {
        SurfacePoint current = path[i], next = path[i + 1];
        
        length = length + sqrt(
                               pow(current.x() + next.x(), 2) +
                               pow(current.y() + next.y(), 2) +
                               pow(current.z() + next.z(), 2)
                               );
    }
    
    return length;
}

