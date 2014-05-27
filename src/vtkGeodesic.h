//
//  vtkGeodesic.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.04.14.
//
//

#ifndef vtkGeodesic_h
#define vtkGeodesic_h

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkIdList.h>
#include <vtkLookupTable.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkUnsignedCharArray.h>

#include "geodesics/geodesic_algorithm_exact.h"

#include "QVTKWidget.h"

#include "vtkShape.h"

//forward declaration due to circular includes
class vtkShape;

using namespace geodesic;
using namespace std;

class vtkGeodesic {
    
    // for using vtkPolyData in the geodesic algorithm
    class geodesicPoints {
    public:
        geodesicPoints(vtkSmartPointer<vtkPolyData> polyData) : polyData_(polyData) {}
        
        vtkIdType size() {
            return polyData_->GetNumberOfPoints() * 3;
        }
        
        double operator[](vtkIdType i) {
            vtkIdType pointId = i / 3;
            vtkIdType component = i % 3;
            double point[3];
            polyData_->GetPoint(pointId, point);
            return point[component];
        }
        
        vtkSmartPointer<vtkPolyData> polyData_;
    };
    
    // for using vtkPolyData in the geodesic algorithm
    class geodesicFaces {
    public:
        geodesicFaces(vtkSmartPointer<vtkPolyData> polyData) : polyData_(polyData) {}
        
        vtkIdType size() {
            return polyData_->GetNumberOfCells() * 3;
        }
        
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
    vtkGeodesic(vtkSmartPointer<vtkShape> shape);
    vtkGeodesic(vtkSmartPointer<vtkShape> shape, unsigned source);
    vtkGeodesic(vtkSmartPointer<vtkShape> shape, vtkSmartPointer<vtkIdList> source);
    ~vtkGeodesic();
    
    // calculate geodesic from a random point to all other points and visualize them
    void    calculateGeodesic_gpu();
    void    visualizeGeodesic(QVTKWidget *qvtkWidget);
    
    void        changeSourcePoint(unsigned source);
    void        changeSourcePoints(vtkSmartPointer<vtkIdList> sources);
    unsigned    findPointFurthestToAllSources();
    
    vtkSmartPointer<vtkIdList>  getVoronoiCells();
    
private:
    void    initialize(unsigned s);
    void    initialize(vtkSmartPointer<vtkIdList> s);
    
    vtkSmartPointer<vtkShape>           shape_;
    Mesh                                mesh_;
    GeodesicAlgorithmExact*             algorithm_;
    geodesicPoints*                     points_;
    geodesicFaces*                      faces_;
    vector<SurfacePoint>                sources_;
    vtkSmartPointer<vtkIdList>          sourceList_;
    
};

#endif
