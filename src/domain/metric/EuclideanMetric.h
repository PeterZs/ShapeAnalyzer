#ifndef __ShapeAnalyzer__EuclideanMetric__
#define __ShapeAnalyzer__EuclideanMetric__

#include <iostream>

#include "Metric.h"
#include "../Shape.h"


#include <vtkPoints.h>
#include <vtkSmartPointer.h>

using namespace attribute;

namespace metric {

///
/// \brief Class for the representation and computation of the euclidean metric on a Shape.
/// \author Emanuel Laude and Zorah Lähner
///
class EuclideanMetric : public Metric {
public:
    EuclideanMetric(Shape* shape) : Metric(shape) {}
    
    virtual double getDistance(vtkIdType a, vtkIdType b);
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source);
    virtual vtkSmartPointer<vtkIdList> getVoronoiCells(vtkSmartPointer<vtkIdList> seeds);
    virtual vtkIdType getFarthestPoint(vtkSmartPointer<vtkIdList> sources);
};
    
}


#endif /* defined(__ShapeAnalyzer__EuclideanMetric__) */
