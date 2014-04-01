#ifndef ShapeAnalyzer_H
#define ShapeAnalyzer_H

#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkCommand.h>
#include <vtkObject.h>
#include <vtkCellPicker.h>
#include <vtkSphereSource.h>
#include <vtkTriangleFilter.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>
#include <vtkBoxWidget.h>
#include <vtkLineSource.h>

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

#include <vector>

#include "ui_ShapeAnalyzer.h"
#include "vtkOFFReader.h"

#define MAX_NUM_ACTORS 10

class ShapeAnalyzer : public QMainWindow, private Ui::ShapeAnalyzer {
    Q_OBJECT

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
                    sa->correspondences[i]->SetPoint1(t->TransformPoint(sa->points[aid]->GetPoint(sa->sources[i].second)));
                    sa->correspondences[i]->Update();
                }
                
                if((sa->targets)[i].first == aid) {
                    sa->correspondences[i]->SetPoint2(t->TransformPoint(sa->points[aid]->GetPoint(sa->targets[i].second)));
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
    virtual void openShape();
    virtual void vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    virtual void toggleBoxWidgets();
    virtual void resetCamera();
    virtual void clear();
private:
    void    setupVTK();
    void    addShapeToVTK(QString fileName);

    int     getActorId(vtkActor* actor);
    bool    eventFilter(QObject *object, QEvent *event);
    
    vtkSmartPointer<vtkActor>       actors[MAX_NUM_ACTORS];
    vtkSmartPointer<vtkBoxWidget>   boxWidgets[MAX_NUM_ACTORS];
    vtkSmartPointer<vtkPolyData>    data[MAX_NUM_ACTORS];
    vtkSmartPointer<vtkPoints>      points[MAX_NUM_ACTORS];
    
    vtkSmartPointer<vtkDataSetMapper>       selectedMapper;
    vtkSmartPointer<vtkActor>               selectedActor;
    vtkSmartPointer<vtkRenderer>            renderer;
    vtkSmartPointer<vtkEventQtSlotConnect>  connections;
    
    std::vector<vtkSmartPointer<vtkActor> >         lines;
    std::vector<vtkSmartPointer<vtkLineSource> >    correspondences;
    std::vector<std::pair<int, vtkIdType> >         sources;
    std::vector<std::pair<int, vtkIdType> >         targets;

    int numberOfActors = 0;
    int actorId = -1;
};

#endif
