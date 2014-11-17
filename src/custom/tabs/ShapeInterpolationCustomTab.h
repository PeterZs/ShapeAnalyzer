////
////  ShapeInterpolationCustomTab.h
////  ShapeAnalyzer
////
////  Created by Emanuel Laude on 24.08.14.
////
////
//
//#ifndef __ShapeAnalyzer__ShapeInterpolationCustomTab__
//#define __ShapeAnalyzer__ShapeInterpolationCustomTab__
//
//#include <vtkActor.h>
//#include <vtkPolyData.h>
//#include <vtkSmartPointer.h>
//#include <vtkRenderer.h>
//
//#include "../../domain/Shape.h"
//#include "../../domain/correspondences/PointCorrespondenceData.h"
//#include "../../util/HashMap.h"
//
//#include "../../view/ShapeAnalyzer.h"
//#include "../../view/CustomListWidgetItem.h"
//
//#include "CustomTab.h"
//
//#include "ui_shapeInterpolation.h"
//
//class ShapeAnalyzer;
//
//class ShapeInterpolationCustomTab : public QWidget, private Ui::ShapeInterpolationWidget, public CustomTab {
//    Q_OBJECT
//    
//public:
//    ShapeInterpolationCustomTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, QWidget* parent, Qt::WindowFlags f = 0);
//    
//    virtual ~ShapeInterpolationCustomTab();
//    
//    virtual void onShapeDelete(Shape* shape);
//    virtual void onShapeAdd(Shape* shape);
//    virtual void onShapeEdit(Shape* shape);
//    virtual void onShapeSelect(Shape* shape);
//    virtual void onClear();
//   
//private slots:
//    virtual void slotInterpolate(int value);
//    virtual void slotChooseShapes();
//    virtual void slotAddShape();
//    
//private:
//    Shape* source_;
//    Shape* target_;
//    
//    /// Interpolated shape.
//    Shape* shape_;
//};
//
//#endif /* defined(__ShapeAnalyzer__ShapeInterpolationCustomTab__) */
