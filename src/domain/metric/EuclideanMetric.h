#ifndef __ShapeAnalyzer__EuclideanMetric__
#define __ShapeAnalyzer__EuclideanMetric__

#include <iostream>

#include "Metric.h"
#include "../Shape.h"


#include <vtkPoints.h>
#include <vtkSmartPointer.h>

///
/// \brief Class for the representation and computation of the euclidean metric on a Shape.
/// \author Emanuel Laude and Zorah Lähner
///
class EuclideanMetric : public Metric {
private:
    EuclideanMetric() {}
    
public:
    /// Returns a new instance of the EuclideanMetric class. This function should not be called directly in your code since it is used internally by the corresponding Factory. Instead use the corresponding Factory class to create new instances of EuclideanMetric.
    static Metric* create() {
        return new EuclideanMetric();
    }
    
    
    /// Returns the unique identifier of this class. Used internally be the corresponding Factory.
    static string getIdentifier() {
        return "euclidean";
    }
    
    virtual double getDistance(vtkIdType a, vtkIdType b);
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source);
    virtual vtkSmartPointer<vtkIdList> getVoronoiCells(vtkSmartPointer<vtkIdList> seeds);
    virtual vtkIdType getFarthestPoint(vtkSmartPointer<vtkIdList> sources);
};

#endif /* defined(__ShapeAnalyzer__EuclideanMetric__) */
