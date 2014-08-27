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
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>

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
#include <algorithm>



#include "CorrespondencePicker.h"
#include "FaceCorrespondencePicker.h"
#include "PointCorrespondencePicker.h"
#include "ShapeAnalyzerInteractorStyle.h"

#include "qt/qtShapeInfoTab.h"
#include "qt/qtListWidgetItem.h"
#include "qt/qtPointCorrespondencesTab.h"
#include "qt/qtFaceCorrespondencesTab.h"
#include "qt/qtCorrespondenceColoringTab.h"
#include "qt/qtShapeInterpolationTab.h"

#include "../domain/Factory.h"
#include "../domain/HashMap.h"
#include "../domain/Shape.h"
#include "../domain/correspondences/Correspondence.h"

#include "../domain/io/SceneWriterReader.h"
#include "../domain/io/vtkOFFReader.h"
#include "../domain/io/vtkToscaASCIIReader.h"

#include "../domain/metric/Metric.h"
#include "../domain/metric/EuclideanMetric.h"
#include "../domain/metric/GeodesicMetric.h"

#include "../domain/signatures/Signature.h"
#include "../domain/signatures/HeatKernelSignature.h"
#include "../domain/signatures/WaveKernelSignature.h"
#include "../domain/signatures/GlobalPointSignature.h"

#include "../domain/segmentation/Segmentation.h"
#include "../domain/segmentation/VoronoiCellSegmentation.h"

#include "../domain/coloring/ScalarPointColoring.h"
#include "../domain/coloring/DiscretePointColoring.h"

#include "../domain/attributes/ScalarPointAttribute.h"
#include "../domain/attributes/DiscretePointAttribute.h"

#include "../domain/FEMLaplaceBeltramiOperator.h"
#include "../domain/HeatDiffusion.h"
#include "../domain/FunctionalMaps.h"

#include "../domain/samplings/Sampling.h"
#include "../domain/samplings/FarthestPointSampling.h"

#include "ui_help.h"
#include "ui_settings.h"
#include "ui_ShapeAnalyzer.h"

using namespace std;

class qtPointCorrespondencesTab;
class qtFaceCorrespondencesTab;
class qtShapeInterpolationTab;

// needed to obtain a ordered sequence of shapes. Result from HashMap is always unsorted. I.e. no statement about the ordering of
// the elements can be made.
struct ShapeComparator {
    bool operator() (Shape* s1, Shape* s2) { return (s1->getId() < s2->getId()); }
};

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

            Shape* shape = sa->shapesByActor_[reinterpret_cast<vtkActor*>(widget->GetProp3D())];

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
        
        SlepcFinalize();
    };
    
    
    // public functions accessable by custom qtTabs
    void showCorrespondence(CorrespondenceData* data);
    void hideCorrespondence(CorrespondenceData* data);
    void deleteCorrespondence(CorrespondenceData* data);
    
    void setSelected(CorrespondenceData* data);
    void samplePointCorrespondences(unsigned int size);
    void sampleFaceCorrespondences(unsigned int size);
    void clearPointCorrespondences();
    void clearFaceCorrespondences();
    
    void vtkAddShape(Shape* shape);
    void showShape(Shape* shape);
    
    void render();
    
    
private slots:
    //QT slots
    virtual void slotExit();
    virtual void slotResetCamera();
    virtual void slotClear();

    virtual void slotShowHelp();
    virtual void slotShowSettings();
    virtual void slotShowContextMenuShapes(const QPoint& pos);
    virtual void slotShowContextMenuCorrespondences(const QPoint& pos);

    virtual void slotClearCurrentSelection();
    virtual void slotHideCorrespondences();

    virtual void slotToggleBoxWidget();
    virtual void slotModeAddCorrespondences();
    
    virtual void slotShowScalarBar(bool);
    
    virtual void slotSetSelectedCurrentShape(QListWidgetItem* current, QListWidgetItem* previous);
    virtual void slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous);
    
    virtual void slotSetShapeDisplayMode();
    virtual void slotSetCorrespondenceType();
    
    virtual void slotSetBackgroundColor();
    virtual void slotToggleProjectionMode();
    
    virtual void slotOpenFile();
    virtual void slotSaveScene();
    virtual void slotExportScene();
    virtual void slotImportCorrespondences();
    virtual void slotExportCorrespondences();
    
    virtual void slotSaveScreenshot();
    
    
    virtual void slotTabShapeInfo(bool);
    virtual void slotTabShapeInterpolation(bool);
    virtual void slotTabCorrespondenceColoring(bool);
    virtual void slotTabAllFaceCorrespondences(bool);
    virtual void slotTabAllPointCorrespondences(bool);
    
    virtual void slotTabCorrespondencesCurrentChanged(int);
    
    //vtk widget slots
    virtual void vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    virtual void vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);

    virtual void vtkKeyPressHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
private:
    //QT
    void qtConnectListCorrespondences();
    void qtConnectListShapes();
    bool eventFilter(QObject *object, QEvent *event);
    
    void qtInitializeSettings();
    
    //Show context menus at global position. Either called from qt slots or from VTK widget (right click on shape/correspondence)
    void qtShowContextMenuShapes(const QPoint& pos, vtkIdType pointId);
    void qtShowContextMenuCorrepondences(const QPoint& pos);
    
    void qtAddMetricMenu(QMenu* menu, HashMap<QAction*, string>& entries);
    void qtAddSignatureMenu(QMenu* menu, HashMap<QAction*, string>& entries);
    void qtAddVoronoiCellsMenu(QMenu* menu, HashMap<QAction*, string>& entries);
    
    void qtInputDialogRenameShape(qtListWidgetItem<Shape>* item);
    void qtInputDialogRenameCorrespondence(qtListWidgetItem<Correspondence>* item);
    void qtInputDialogOpacity(Shape* shape);
    void qtShowEigenfunction(Shape* shape);
    void qtShowHeatDiffusion(Shape* shape);
    void qtShowSignature(string id, Shape* shape);
    void qtShowMetricColoring(string id, Shape* shape);
    void qtShowVoronoiCells(string metricId, Shape* shape);
    void qtCreateIdentityCorrespondences(Shape* shape1);
    void qtTransferCoordinateFunction(Shape* shape1);
    void qtCreateShapeSegment(Shape *shape, vtkIdType pointId);
    
    
    //vtk
    void vtkCorrespondenceClicked(Correspondence* correspondence, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkShapeClicked(Shape* shape, vtkIdType pointId, vtkIdType faceId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkSetup();
    void vtkOpenShape(vtkPolyDataAlgorithm* reader, string name);
    void vtkOpenScene(string filename);
    void vtkSaveScene(string filename);
    void vtkImportScene(string filename);
    void vtkExportScene(string filename);

    

    void clear();
    void deleteShape(int i);
    void deleteCorrespondence(int i);
    void hideCorrespondence(int i);
    void hideCorrespondences(); // hides currently displayed correspondences
    void addShape(Shape* shape);
    void addCorrespondence();
    

    
    
    //index shapes & correspondences by their actors. Faster access in linear time worst case. Usually constant time.
    HashMap<vtkActor*, Shape*> shapesByActor_;
    
    HashMap<vtkActor*, FaceCorrespondence*> faceCorrespondencesByActor_;
    HashMap<vtkActor*, PointCorrespondence*> pointCorrespondencesByActor_;
    
    // all face and point correspondences data, the bool indicates if for the data there exists a corresponding Correspondence object in "...CorrespondencesByActor_". In case we are in "view PointCorrespondences mode" this correspondence object is also visible in the qvtkWidget
    HashMap<FaceCorrespondenceData*, bool> faceCorrespondenceData_;    
    HashMap<PointCorrespondenceData*, bool> pointCorrespondenceData_;

    //vtk stuff
    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkEventQtSlotConnect> connections_;
    
    
    CorrespondencePicker* correspondencePicker_;
    
    //QT
    QActionGroup* actionGroupCorrespondenceType_;
    QActionGroup* actionGroupMode_;
    QActionGroup* actionGroupShapeDisplayMode_;
    QActionGroup* actionGroupProjectionMode_;
    
    Ui_Settings uiSettings_;
    QDialog*    dialogSettings_;
    
    
    
    HashMap<Shape*, vtkSmartPointer<vtkIdList> > segmentations_;
    
    HashMap<string, qtCorrespondencesTab*> correspondencesTabs_;
    HashMap<string, qtShapesTab*> shapesTabs_;
    
    //counter for ids
    int lastInsertShapeID_;
    int lastInsertCorresondenceID_; //correspondence data id
    
    int pickerCounter_;
    
    
    vtkSmartPointer<vtkScalarBarActor> scalarBar_;
};




#endif
