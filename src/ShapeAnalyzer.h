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
#include <vtkLineSource.h>
#include <vtkObject.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTriangleFilter.h>
#include <vtkUnstructuredGrid.h>

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

#include <list>

#include "Correspondence.h"
#include "Shape.h"
#include "ShapeListItem.h"
#include "CorrespondenceListItem.h"
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
            
            Shape* shape = sa->findShapeByActor(reinterpret_cast<vtkActor*>(widget->GetProp3D()));
            
            if(shape == sa->selectedShape) {
                sa->selectedActor->SetUserTransform(t);
            }

            for(list<Correspondence*>::iterator it = sa->correspondences_.begin(); it != sa->correspondences_.end(); it++) {
                if((*it)->getShape1() == shape) {
                    (*it)->transformPoint1(t);
                }
                
                if((*it)->getShape2() == shape) {
                    (*it)->transformPoint2(t);
                }
            }
        }
    };
    
public:
    ShapeAnalyzer();
    ~ShapeAnalyzer() {};

private slots:
    virtual void exit();
    virtual void resetCamera();
    virtual void clear();

    virtual void openShape();

    virtual void openHelpWindow();
    virtual void showContextMenuShapes(const QPoint&);
    virtual void showContextMenuCorrespondences(const QPoint&);

    virtual void deleteMarkedCorrespondence();
    virtual void vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);

    virtual void toggleBoxWidget();
    
    virtual void setCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous);
    virtual void setCurrentCorrespondenceColor(QListWidgetItem* current, QListWidgetItem* previous);

private:
    void setupVTK();
    void connectListCorrespondences();
    void connectListShapes();
    
    Shape* findShapeByActor(vtkActor* actor);
    Shape* addShapeToVTK(QString fileName);
    Correspondence* addCorrespondenceToVTK(Shape* shape1, Shape* shape2, double* point1, double* point2, vtkLinearTransform* t1, vtkLinearTransform* t2);
    
    void deleteCorrespondence(int i);
    void deleteShape(int i);
    bool eventFilter(QObject *object, QEvent *event);

    list<Shape*> shapes_;
    list<Correspondence*> correspondences_;
    
    
    //Correspondence selection stuff
    bool selected_; //flag indicating that a triangle has been selected on first shape. Wait for selection of corresponding triangle on another shape
    Shape* selectedShape; //Pointer to shape that was selected in add-matches-mode
    vtkSmartPointer<vtkDataSetMapper> selectedMapper; // mapper of green triangle that corresponds to selected triangle of selected shape
    vtkSmartPointer<vtkActor> selectedActor; // actor representing green triangle that corresponds to selected triangle of selected shape
    pair<Shape*, vtkSmartPointer<vtkPoints> > source_; // used to remember wheter a node was already selected previously when selecting correspondences
    
    
    
    //vtk stuff
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkEventQtSlotConnect> connections;
    
    
    
    // only used for naming in qt
    int lastInsertShapeID_ = 0;
    int lastInsertCorresondenceID_ = 0;
};

#endif
