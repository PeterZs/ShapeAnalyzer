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
            
            if(sa->waitForTriangleSelection_ == true && shape == sa->selectedTriangleShape_) {
                sa->selectedTriangleActor_->SetUserTransform(t);
            }

            for(list<Correspondence*>::iterator it = sa->correspondences_.begin(); it != sa->correspondences_.end(); it++) {
                if((*it)->getShape1() == shape) {
                    (*it)->transformTriangleCenter1(t);
                }
                
                if((*it)->getShape2() == shape) {
                    (*it)->transformTriangleCenter2(t);
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

    virtual void deleteSelectedTriangle();
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
    Correspondence* addCorrespondenceToVTK(Shape* shape1, Shape* shape2, vtkSmartPointer<vtkTriangle> triangle1, vtkSmartPointer<vtkTriangle> triangle2);
    
    void deleteCorrespondence(int i);
    void deleteShape(int i);
    bool eventFilter(QObject *object, QEvent *event);

    list<Shape*> shapes_;
    list<Correspondence*> correspondences_;
    
    
    //Correspondence creation stuff
    bool waitForTriangleSelection_; //flag indicating that a triangle has been selected on first shape. Wait for selection of corresponding triangle on another shape
    Shape* selectedTriangleShape_; //Pointer to shape that was selected in add-Correspondeces-Mode and therefore has the green triangle on it. Required to be declared in the class to delete green triangle in case the selected shape is deleted in method deleteShape(int i)
    vtkSmartPointer<vtkDataSetMapper> selectedTriangleMapper_; // mapper of green triangle of correspondence selection that maps the triangle data to the triangle actor
    vtkSmartPointer<vtkActor> selectedTriangleActor_; // actor representing green triangle of selected shape
    vtkSmartPointer<vtkTriangle> triangle1_; //Remember first selection.
    Shape* shape1_; //Remember first selection. Be careful: shape1 is not necessarily equal to selectedShape which holds the shape that is currently selected.
    
    //vtk stuff
    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkEventQtSlotConnect> connections_;
    
    
    
    // only used for naming in qt
    int lastInsertShapeID_ = 0;
    int lastInsertCorresondenceID_ = 0;
};

#endif
