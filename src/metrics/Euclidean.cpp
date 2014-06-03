//
//  Euclidean.cpp
//  ShapeAnalyzer
//
//  Created by Zorah on 27.05.14.
//
//

#include "Euclidean.h"

Euclidean::Euclidean(Shape* shape) {
    identifier_ = "Euclidean";
    shape_ = shape;
}

double Euclidean::getDistance(unsigned int a, unsigned int b) {
    double p1[3], p2[3];
    shape_->getPolyData()->GetPoints()->GetPoint(a, p1);
    shape_->getPolyData()->GetPoints()->GetPoint(b, p2);
    
    return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}

vector<double> Euclidean::getAllDistances(unsigned source) {
    vector<double> distances;
    distances.resize(shape_->getPolyData()->GetPoints()->GetNumberOfPoints());
    
    for(int i = 0; i < shape_->getPolyData()->GetPoints()->GetNumberOfPoints(); i++) {
        distances[i] = getDistance(source, i);
    }
    
    return distances;
}
