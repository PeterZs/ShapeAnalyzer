#ifndef ShapeAnalyzer_H
#define ShapeAnalyzer_H

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkExtractSelection.h>
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

    // manages click responses for correspondence setting
    // TODO ist doch so manu? :D
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

            for(int i = 0; i < sa->correspondences.size(); i++) {
                if((sa->sources)[i].first == aid) {
                    sa->correspondences[i]->SetPoint1(t->TransformPoint(sa->shapes_[aid].getPoints()->GetPoint(sa->sources[i].second)));
                    sa->correspondences[i]->Update();
                }
                
                if((sa->targets)[i].first == aid) {
                    sa->correspondences[i]->SetPoint2(t->TransformPoint(sa->shapes_[aid].getPoints()->GetPoint(sa->targets[i].second)));
                    sa->correspondences[i]->Update();
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

    virtual void showContextMenu(const QPoint&);

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

    void    deleteShape(int i);

    int     getActorId(vtkActor* actor);
    bool    eventFilter(QObject *object, QEvent *event);

    vector<Shape>               shapes_;
    // TODO go with more efficient data structure here
    vector<Correspondence>      correspondences_; 
    
    vtkSmartPointer<vtkDataSetMapper>       selectedMapper;
    vtkSmartPointer<vtkActor>               selectedActor;
    vtkSmartPointer<vtkRenderer>            renderer;
    vtkSmartPointer<vtkEventQtSlotConnect>  connections;
    
    vector<vtkSmartPointer<vtkActor> >         lines;
    vector<vtkSmartPointer<vtkLineSource> >    correspondences;
    vector<pair<int, vtkIdType> >         sources;
    vector<pair<int, vtkIdType> >         targets;

    int numberOfActors = 0;
    int actorId = -1;
};

#endif
