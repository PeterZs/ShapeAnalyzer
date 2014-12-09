#ifndef __ShapeAnalyzer__IdentityMatchingTab__
#define __ShapeAnalyzer__IdentityMatchingTab__

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

#include <QMessageBox>

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../util/HashMap.h"

#include "../../view/ShapeAnalyzerInterface.h"
#include "../../view/CustomListWidgetItem.h"

#include "CustomTab.h"

#include "ui_IdentityMatchingTabWidget.h"

/// \brief Tab to match same point/face ids on two shapes.
/// \details If two different shapes have been selected points/faces with the same ids will be matched together.
/// If a lower density is selected the ids will be selected evenly (e.g. every second id). The shapes do not
/// need to have the same number of points/faces, the lower number will be chosen as a reference. 
class IdentityMatchingTab : public QWidget, private Ui::IdentityMatchingTabWidget, public CustomTab {
    Q_OBJECT
    
public:
    IdentityMatchingTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, ShapeAnalyzerInterface* shapeAnalyzer);
    
    virtual ~IdentityMatchingTab();
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();
private slots:
    virtual void slotMatch();
};

#endif /* defined(__ShapeAnalyzer__IdentityMatchingTab__) */
