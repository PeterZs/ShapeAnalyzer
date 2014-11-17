#ifndef __ShapeAnalyzer__GeodesicMetric__
#define __ShapeAnalyzer__GeodesicMetric__

#include <iostream>

#include "Metric.h"
#include "../Shape.h"

#include <vtkCellData.h>
#include <vtkIdList.h>
#include <vtkLookupTable.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>

#include "3rdparty/geodesic/geodesic_algorithm_exact.h"
#include "3rdparty/geodesic/geodesic_error.h"

using namespace geodesic;
using namespace std;
using namespace attribute;

namespace metric {

///
/// \brief Class for the representation and computation of the geodesic metric on a Shape.
/// \details This class is a wrapper class for the implementation of the exact geodesic metric obtained from https://code.google.com/p/geodesic/.
/// \author Emanuel Laude and Zorah Lähner
///
class GeodesicMetric : public Metric {
private:
    /// \brief Nested class for the efficient transfer of the vertex data from the
    /// vtk data structures to the data structures of the geodesic library.
    ///
    class geodesicPoints {
    public:
        /// Contructor. Takes the shapes polyData as an argument.
        geodesicPoints(vtkSmartPointer<vtkPolyData> polyData) : polyData_(polyData) {}
        
        /// returns the size of the 1D array containing all the vertex coordinates
        vtkIdType size() {
            return polyData_->GetNumberOfPoints() * 3;
        }
        
        /// returns the (i % 3)-th component of the (i / 3)-th vertex.
        double operator[](vtkIdType i) {
            vtkIdType pointId = i / 3;
            vtkIdType component = i % 3;
            double point[3];
            polyData_->GetPoint(pointId, point);
            return point[component];
        }
        
        vtkSmartPointer<vtkPolyData> polyData_;
    };
    
    /// \brief Nested class for the efficient transfer of the face data from the
    /// vtk data structures to the data structures of the geodesic library.
    ///
    class geodesicFaces {
    public:
        /// Contructor. Takes the shapes polyData as an argument.
        geodesicFaces(vtkSmartPointer<vtkPolyData> polyData) : polyData_(polyData) {}
        
        /// Returns the size of the 1D array containing all the vertex coordinates
        vtkIdType size() {
            return polyData_->GetNumberOfCells() * 3;
        }
        
        /// Returns the (i % 3)-th vertex ID of the (i / 3)-th face.
        vtkIdType operator[](vtkIdType i) {
            vtkIdType cellId = i / 3;
            vtkIdType pointId = i % 3;
            
            vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
            polyData_->GetCellPoints(cellId, ids);
            return ids->GetId(pointId);
        }
        
        vtkSmartPointer<vtkPolyData> polyData_;
    };
    
public:
    GeodesicMetric(Shape* shape);
    
    /// Empty destructor.
    virtual ~GeodesicMetric();
    
    virtual double getDistance(vtkIdType a, vtkIdType b);
    virtual void getAllDistances(ScalarPointAttribute& distances, vtkIdType source);
    virtual vtkSmartPointer<vtkIdList> getVoronoiCells(vtkSmartPointer<vtkIdList> seeds);
    virtual vtkIdType getFarthestPoint(vtkSmartPointer<vtkIdList> sources);
private:
    /// @{
    Mesh                                mesh_;
    GeodesicAlgorithmExact*             algorithm_;
    geodesicPoints*                     points_;
    geodesicFaces*                      faces_;
    /// @}
};
    
}
    

#endif /* defined(__ShapeAnalyzer__GeodesicMetric__) */
