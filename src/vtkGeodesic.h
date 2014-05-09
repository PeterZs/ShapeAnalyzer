//
//  vtkGeodesic.h
//  ShapeAnalyzer
//
//  Created by Zorah on 27.04.14.
//
//

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkUnsignedCharArray.h>
#include <vtkLookupTable.h>

#include "geodesics/geodesic_algorithm_exact.h"

#include "QVTKWidget.h"
#include "Shape.h"

#ifndef vtkGeodesic_h
#define vtkGeodesic_h

class vtkGeodesic {
    class geodesicPoints {
    public:
        geodesicPoints(vtkSmartPointer<vtkPolyData> polyData) : polyData_(polyData) {};
        
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
    
    class geodesicFaces {
    public:
        geodesicFaces(vtkSmartPointer<vtkPolyData> polyData) : polyData_(polyData) {};
        
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
    vtkGeodesic(Shape *shape);
    vtkGeodesic(Shape *shape, unsigned source);
    ~vtkGeodesic() {};
    
    // calculate geodesic from a random point to all other points and visualize them
    void    calculateGeodesic_gpu();
    void    changeSourcePoint(unsigned source);
    void    visualizeGeodesic(QVTKWidget *qvtkWidget);
    
private:
    void    initialize();
    
    Shape*                              shape_;
    geodesic::Mesh                      mesh_;
    geodesic::GeodesicAlgorithmExact*   algorithm_;
    unsigned                            source_;
    
};

#endif
