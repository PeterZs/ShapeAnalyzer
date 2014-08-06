//
//  LaplaceBeltramiOperator.cpp
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 28.07.14.
//
//

#include "LaplaceBeltramiOperator.h"

LaplaceBeltramiOperator::LaplaceBeltramiOperator(vtkSmartPointer<vtkPolyData> polyData, int numberOfEigenfunctions) : polyData_(polyData), numberOfEigenfunctions_(numberOfEigenfunctions) {
}