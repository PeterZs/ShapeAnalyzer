#ifndef __ShapeAnalyzer__FunctionTransferTab__
#define __ShapeAnalyzer__FunctionTransferTab__

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

#include <QMessageBox>
#include <QErrorMessage>

#include "../../domain/Shape.h"
#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../domain/metric/GeodesicMetric.h"
#include "../../domain/laplaceBeltrami/PetscFEMLaplaceBeltramiOperator.h"
#include "../../domain/signatures/PetscWaveKernelSignature.h"
#include "../../domain/PetscFunctionalMaps.h"
#include "../../util/HashMap.h"

#include "../../view/CustomListWidgetItem.h"

#include "CustomTab.h"

#include "ui_FunctionTransferTabWidget.h"

class FunctionTransferTab : public QWidget, private Ui::FunctionTransferTabWidget, public CustomTab {
    Q_OBJECT
    
public:
    FunctionTransferTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences,
                        ShapeAnalyzerInterface* shapeAnalyzer);
    
    virtual ~FunctionTransferTab();
    
    virtual void onShapeDelete(Shape* shape);
    virtual void onShapeAdd(Shape* shape);
    virtual void onShapeEdit(Shape* shape);
    virtual void onClear();
private slots:
    virtual void slotTransfer();
};

#endif /* defined(__ShapeAnalyzer__FunctionTransferTab__) */
