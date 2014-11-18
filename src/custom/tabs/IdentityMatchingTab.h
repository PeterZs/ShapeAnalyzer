////
////  IdentityMatchingTab.h
////  ShapeAnalyzer
////
////  Created by Emanuel Laude on 24.08.14.
////
////
//
//#ifndef __ShapeAnalyzer__IdentityMatchingTab__
//#define __ShapeAnalyzer__IdentityMatchingTab__
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
//#include "ui_IdentityMatchingTabWidget.h"
//
//class ShapeAnalyzer;
//
//class IdentityMatchingTab : public QWidget, private Ui::IdentityMatchingTabWidget, public CustomTab {
//    Q_OBJECT
//    
//public:
//    IdentityMatchingTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent);
//    
//    virtual ~IdentityMatchingTab();
//    
//    virtual void onShapeDelete(Shape* shape);
//    virtual void onShapeAdd(Shape* shape);
//    virtual void onShapeEdit(Shape* shape);
//    virtual void onClear();
//
//    static CustomTab* create(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent) {
//        return new IdentityMatchingTab(shapes, pointCorrespondences, faceCorrespondences, parent);
//    }
//    
//private slots:
//    virtual void slotMatch();
//    
//private:
//    Shape* shape1_;
//    Shape* shape2_;
//};
//
//#endif /* defined(__ShapeAnalyzer__IdentityMatchingTab__) */
