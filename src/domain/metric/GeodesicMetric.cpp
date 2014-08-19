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
    vector<geodesic::SurfacePoint> all_sources(1,source);
    try {
        algorithm_->propagate(all_sources);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
    }
    
    for(vtkIdType i = 0; i< mesh_.vertices().size(); ++i) {
        geodesic::SurfacePoint p(&mesh_.vertices()[i]);
        
        double distance;
        try {
        algorithm_->best_source(p,distance);		//for a given surface point, find closets source and distance to this source
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
    
    sourceList_ = vtkSmartPointer<vtkIdList>::New();
    sources_ = vector<SurfacePoint>();
    
    try {
        algorithm_->geodesic(source, target, path);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
        return 0.0;
    }
    
    return calculateLengthOfPath(path);
}


///////////////////////////////////////////////////////////////////////////////
// Returns the point that is farthest away from all points in sources
// Notice that this will remove the current sources and precomputed information
// about these if the input list is not the same as the current source list
vtkIdType GeodesicMetric::getPointFarthestFromAllSources(vtkSmartPointer<vtkIdList> sources) {
    vtkIdType number = sourceList_->GetNumberOfIds();
    sourceList_->IntersectWith(sources);
    
    // sources are not the same as the given list, recompute
    if (sourceList_->GetNumberOfIds() < number || sources->GetNumberOfIds() != number) {
        // recompute geodesics
        changeSourcePoints(sources);
    }
    
    return getPointFarthestFromAllSources();
}


///////////////////////////////////////////////////////////////////////////////
// Geodesic Public Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// changes the source to points with id s
// other already computed distances will be lost
void GeodesicMetric::changeSourcePoint(vtkIdType s) {
    vtkSmartPointer<vtkIdList> list = vtkSmartPointer<vtkIdList>::New();
    list->InsertNextId(s);
    
    changeSourcePoints(list);
}


///////////////////////////////////////////////////////////////////////////////
// changes the source to points with ids in s
// other already computed distances will be lost
void GeodesicMetric::changeSourcePoints(vtkSmartPointer<vtkIdList> s) {
    sourceList_ = s;
    sources_.resize(s->GetNumberOfIds());
    
    // create SurfacePoints for all ids
    for(int i = 0; i < s->GetNumberOfIds(); i++) {
        SurfacePoint source(&mesh_.vertices()[s->GetId(i)]); //create source
        sources_[i] = source;
    }
    
    // recomputation of geodesics
    try {
        algorithm_->propagate(sources_);
    } catch (geodesic_error& e) {
        cout << e.what() << '\n';
    }
}


///////////////////////////////////////////////////////////////////////////////
// Returns the point that is farthest away from all points in sources
vtkIdType GeodesicMetric::getPointFarthestFromAllSources() {
    double distance = 0.0;
    
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
        if(dist > distance) {
            distance = dist;
            id = i;
        }
        
    }
    
    return id;
}

///////////////////////////////////////////////////////////////////////////////
// Geodesic Private Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// calculates the length of the path given by the SurfacePoints in path
double GeodesicMetric::calculateLengthOfPath(vector<SurfacePoint> path) {
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

