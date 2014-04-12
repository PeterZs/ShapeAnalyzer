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

#include <vector>

#include "Shape.h"
#include "Correspondence.h"

#include "ui_ShapeAnalyzer.h"
#include "vtkOFFReader.h"

#define MAX_NUM_ACTORS 10

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
            
            int aid = sa->getActorId(reinterpret_cast<vtkActor*>(widget->GetProp3D()));
            
            if(sa->numberOfActors != 0 && aid == sa->actorId) {
                sa->selectedActor->SetUserTransform(t);
            }

            for(int i = 0; i < sa->correspondences_.size(); i++) {
                int shape1Id = sa->getActorId(sa->correspondences_[i].getShape1().getActor());
                int shape2Id = sa->getActorId(sa->correspondences_[i].getShape2().getActor());
                if(shape1Id == aid) {
                    sa->correspondences_[i].setPoint1(t->TransformPoint(sa->correspondences_[i].getPoint1()));
                    sa->correspondences_[i].updateLine();
                }
                
                if(shape2Id == aid) {
                    sa->correspondences_[i].setPoint2(t->TransformPoint(sa->correspondences_[i].getPoint2()));
                    sa->correspondences_[i].updateLine();
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

    virtual void showContextMenuShapes(const QPoint&);
    virtual void showContextMenuCorrespondences(const QPoint&);

    virtual void deleteMarkedCorrespondence();
    virtual void vtkClickHandler
                (
                    vtkObject *caller, 
                    unsigned long vtkEvent, 
                    void *clientData, 
                    void *callData, 
                    vtkCommand *command
                );
    virtual void toggleBoxWidgets();
    virtual void toggleCurrent();

private:
    void    setupVTK();
    void    addShapeToVTK(QString fileName);

    void    deleteCorrespondence(int i);
    void    deleteShape(int i);
    void    deleteShape(Shape shape);

    int     getActorId(vtkActor* actor);
    bool    eventFilter(QObject *object, QEvent *event);

    // TODO go with more efficient data structure here
    // the indices of an actor in the vectors are not static
    vector<Shape>               shapes_;
    vector<Correspondence>      correspondences_; 
    
    vtkSmartPointer<vtkDataSetMapper>           selectedMapper;
    vtkSmartPointer<vtkActor>                   selectedActor;
    vtkSmartPointer<vtkRenderer>                renderer;
    vtkSmartPointer<vtkEventQtSlotConnect>      connections;
    // used to remember wheter a node was already selected previously when
    // selecting correspondences
    pair<Shape, vtkSmartPointer<vtkPoints> >    source_;
    bool                                        set_;

    // only used for naming in qt
    // this is NOT the real number of actors/correspondences
    // use the vector size instead
    // TODO might think of a better way for naming
    int numberOfActors = 0;
    int numberOfCorrespondences = 0;

    int actorId = -1;
};

#endif
