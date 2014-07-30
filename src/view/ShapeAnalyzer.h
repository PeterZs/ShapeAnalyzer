#ifndef ShapeAnalyzer_H
#define ShapeAnalyzer_H

#include <vtkActor.h>
#include <vtkAlgorithmOutput.h>
#include <vtkBoxWidget.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkExtractSelection.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkObject.h>
#include <vtkPNGWriter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTriangleFilter.h>
#include <vtkSelectionNode.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointSet.h>
#include <vtkWindowToImageFilter.h>

#include <QActionGroup>
#include <QCheckBox>
#include <QInputDialog>
#include <QKeySequence>
#include <QList>
#include <QListWidget>
#include <QMainWindow>
#include <QShortcut>
#include <QString>
#include <QFileDialog>
#include <QColorDialog>

#include <exception>
#include <unordered_map>


#include "CorrespondencePicker.h"
#include "FaceCorrespondencePicker.h"
#include "PointCorrespondencePicker.h"

#include "qt/qtShapeInfoTab.h"
#include "qt/qtListWidgetItem.h"
#include "qt/qtCorrespondenceWindow.h"

#include "../domain/correspondences/Correspondence.h"
#include "../domain/io/SceneReader.h"
#include "../domain/io/vtkOFFReader.h"
#include "../domain/io/vtkToscaASCIIReader.h"
#include "../domain/Set.h"
#include "../domain/Shape.h"
#include "../domain/metric/Metric.h"
#include "../domain/metric/MetricFactory.h"
#include "../domain/samplings/SamplingFactory.h"

#include "../domain/coloring/MetricColoring.h"

#include "../domain/FEMLaplaceBeltramiOperator.h"

#include "ui_help.h"
#include "ui_settings.h"
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

            // transform correspondences
            for(auto it = sa->pointCorrespondencesByActor_.begin(); it != sa->pointCorrespondencesByActor_.end(); it++) {
                it->second->transform(shape);
            }
            for(auto it = sa->faceCorrespondencesByActor_.begin(); it != sa->faceCorrespondencesByActor_.end(); it++) {
                it->second->transform(shape);
            }
            
            sa->qvtkWidget->GetRenderWindow()->Render();
        }
    };
    
public:
    ShapeAnalyzer();
    ~ShapeAnalyzer() {
        delete correspondencePicker_;
        delete dialogSettings_;
    };
    
    QList<QListWidgetItem *> getShapes();
    
private slots:
    //QT slots
    virtual void slotExit();
    virtual void slotResetCamera();
    virtual void slotClear();

    virtual void slotOpenFile();
    virtual void slotLoadCorrespondences();

    virtual void slotOpenHelpWindow();
    virtual void slotOpenCorrespondenceWindowPoints();
    virtual void slotOpenCorrespondenceWindowFaces();
    virtual void slotShowSettings();
    virtual void slotShowContextMenuShapes(const QPoint& pos);
    virtual void slotShowContextMenuCorrespondences(const QPoint& pos);

    virtual void slotClearCurrentSelection();

    virtual void slotToggleBoxWidget();
    virtual void slotAddCorrespondencesMode();
    virtual void slotEndCorrespondencePicker();
    
    virtual void slotShapeSelectionChanged(QListWidgetItem* current, QListWidgetItem* previous);
    
    virtual void slotSetCurrentBoxWidget(QListWidgetItem* current, QListWidgetItem* previous);
    virtual void slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous);
    
    virtual void slotSetShapeDisplayMode();
    virtual void slotSetCorrespondenceType();
    
    virtual void slotSetBackgroundColor();
    virtual void slotTogglePerspectiveMode(bool);
    virtual void slotToggleParallelMode(bool);
    
    virtual void slotSaveScene();
    virtual void slotExportScene();
    virtual void slotExportCorrespondences();
    virtual void slotSaveImage();
    
    virtual void slotTabShapeInfo(bool);
    
    //vtk widget slots
    virtual void vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    virtual void vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    
private:
    //QT
    void qtConnectListCorrespondences();
    void qtConnectListShapes();
    bool eventFilter(QObject *object, QEvent *event);
    
    void qtInitializeSettings();
    
    //Show context menus at global position. Either called from qt slots or from VTK widget (right click on shape/correspondence)
    void qtShowContextMenuShapes(const QPoint& pos);
    void qtShowContextMenuCorrepondences(const QPoint& pos);
    
    vector<QAction*> qtAddMetricMenu(QMenu* menu);
    vector<QAction*> qtAddSamplingMenu(QMenu* menu);
    
    void qtInputDialogFPS();
    void qtInputDialogRename(QListWidgetItem* item);
    void qtInputDialogOpacity(Shape* shape);
    
    //vtk
    void vtkCorrespondenceClicked(Correspondence* correspondence, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkShapeClicked(Shape* shape, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkSetup();
    void vtkAddShape(Shape* shape);
    void vtkOpenShape(vtkPolyDataAlgorithm* reader);
    void vtkOpenScene(string filename);
    void vtkSaveScene(string filename);
    void vtkImportScene(string filename);
    void vtkExportScene(string filename);
    
    Correspondence*     findCorrespondenceByActor(vtkActor* actor);
    Shape*              findShapeByActor(vtkActor* actor);
    
    // these are expensive, use with care
    Correspondence*         findCorrespondenceByData(CorrespondenceData* data);
    FaceCorrespondence*     findFaceCorrespondenceByData(FaceCorrespondenceData* data);
    PointCorrespondence*    findPointCorrespondenceByData(PointCorrespondenceData* data);
    
    void clear();
    void clearCorrespondences();
    void deleteCorrespondence(int i, bool deleteData);
    void deleteShape(int i);
    void addShape(Shape* shape);
    void addCorrespondence();

    //index shapes & correspondences by their actors. unordered_map corresponds to hashmap. Faster access in linear time worst case. Usually constant time.
    Set<vtkActor*, Shape*> shapesByActor_;
    
    unordered_map<vtkActor*, FaceCorrespondence*> faceCorrespondencesByActor_;
    unordered_map<vtkActor*, PointCorrespondence*> pointCorrespondencesByActor_;
    
    // all face and point correspondences data, the bool indicates if the data is visible
    Set<PointCorrespondenceData*, bool> pointData_;
    Set<FaceCorrespondenceData*, bool> faceData_;

    //vtk stuff
    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkEventQtSlotConnect> connections_;
    
    
    CorrespondencePicker* correspondencePicker_;
    
    
    //QT
    QActionGroup* actionGroupCorrespondenceType;
    QActionGroup* actionGroupMode;
    QActionGroup* actionGroupShapeDisplayMode;
    
    Ui_Settings uiSettings_;
    QDialog*    dialogSettings_;
    
    //counter for ids
    int lastInsertShapeID_;
    int lastInsertCorresondenceID_;
    
    int pickerCounter_;
};

#endif