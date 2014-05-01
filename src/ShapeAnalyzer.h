#ifndef ShapeAnalyzer_H
#define ShapeAnalyzer_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkExtractSelection.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkObject.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTriangleFilter.h>
#include <vtkSelectionNode.h>

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

#include <unordered_map>

#include "Correspondence.h"
#include "CorrespondenceListItem.h"
#include "Shape.h"
#include "ShapeListItem.h"
#include "CorrespondenceListItem.h"
#include "CorrespondencePicker.h"
#include "vtkGeodesic.h"
#include "vtkOFFReader.h"

#include "ui_help.h"
#include "ui_ShapeAnalyzer.h"

using namespace std;

class ShapeAnalyzer : public QMainWindow, private Ui::ShapeAnalyzer {
    Q_OBJECT

    // manages update of correspondences after scene transformation
    class vtkBoxWidgetCallback : public vtkCommand {
    public:
        ShapeAnalyzer *sa;
        
        static vtkBoxWidgetCallback *New() {
            return new vtkBoxWidgetCallback;
        }
        
        virtual void Execute(vtkObject *caller, unsigned long, void*) {
            vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
            vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
            widget->GetTransform(t);
            widget->GetProp3D()->SetUserTransform(t);
            
            unordered_map<vtkActor*, Shape*>::iterator it = sa->shapesByActor_.find(reinterpret_cast<vtkActor*>(widget->GetProp3D()));
            Shape* shape = nullptr;
            if(it != sa->shapesByActor_.end()) {
                shape = it->second;
            }

            for(unordered_map<vtkActor*, Correspondence*>::iterator it = sa->correspondencesByActor_.begin(); it != sa->correspondencesByActor_.end(); it++) {
                if(it->second->getShape1() == shape) {
                    it->second->transformPoint1(t);
                }
                
                if(it->second->getShape2() == shape) {
                    it->second->transformPoint2(t);
                }
            }
            sa->qvtkWidget->GetRenderWindow()->Render();
        }
    };
    
public:
    ShapeAnalyzer();
    ~ShapeAnalyzer() {
        delete correspondencePicker_;
    };
    
private slots:
    //QT slots
    virtual void slotExit();
    virtual void slotResetCamera();
    virtual void slotClear();

    virtual void slotOpenShape();

    virtual void slotOpenHelpWindow();
    virtual void slotShowContextMenuShapes(const QPoint& pos);
    virtual void slotShowContextMenuCorrespondences(const QPoint& pos);

    virtual void slotClearCurrentSelection();

    virtual void slotToggleBoxWidget();
    
    virtual void slotSetCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous);
    virtual void slotSetCurrentCorrespondenceColor(QListWidgetItem* current, QListWidgetItem* previous);
    
    
    //vtk widget slots
    virtual void vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    virtual void vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
private:
    //QT
    void qtConnectListCorrespondences();
    void qtConnectListShapes();
    bool eventFilter(QObject *object, QEvent *event);
    
    //Show context menus at global position. Either called from qt slots or from VTK widget (right click on shape/correspondence)
    void qtShowContextMenuShapes(const QPoint& pos);
    void qtShowContextMenuCorrepondences(const QPoint& pos);
    
    //vtk
    void vtkCorrespondenceClicked(Correspondence* correspondence, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkShapeClicked(Shape* shape, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    
    void vtkSetup();
    
    Shape* vtkAddShape(QString fileName);
    
    void clear();
    void deleteCorrespondence(int i);
    void deleteShape(int i);

    unordered_map<vtkActor*, Shape*> shapesByActor_;
    unordered_map<vtkActor*, Correspondence*> correspondencesByActor_;

    //vtk stuff
    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkEventQtSlotConnect> connections_;
    
    
    CorrespondencePicker* correspondencePicker_;
    
    
    // only used for naming in qt
    int lastInsertShapeID_ = 0;
    int lastInsertCorresondenceID_ = 0;
};

#endif
