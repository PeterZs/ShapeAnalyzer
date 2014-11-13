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
#include <vtkPLYReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYWriter.h>
#include <vtkGL2PSExporter.h>

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
#include "qt/qtMeshCheckTab.h"

#include "../domain/Factory.h"
#include "../domain/HashMap.h"
#include "../domain/Shape.h"
#include "../domain/correspondences/Correspondence.h"

#include "../domain/io/SceneWriterReader.h"
#include "../domain/io/vtkOFFReader.h"
#include "../domain/io/vtkToscaReader.h"
#include "../domain/io/vtkOFFWriter.h"
#include "../domain/io/vtkToscaWriter.h"
#include "../domain/io/vtkOBJWriter.h"


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
#include "ui_saveScreenshot.h"
#include "ui_ShapeAnalyzer.h"

using namespace std;

class qtPointCorrespondencesTab;
class qtFaceCorrespondencesTab;
class qtShapeInterpolationTab;


///
/// \brief Manages the interaction with the GUI.
/// \details TODO blabla
///
/// \author Emanuel Laude and Zorah Lähner
///

class ShapeAnalyzer : public QMainWindow, private Ui::ShapeAnalyzer {
    Q_OBJECT
    
    /// \brief needed to obtain a ordered sequence of shapes. Result from HashMap is always unsorted. I.e. no statement about the ordering of
    /// the elements can be made.
    struct ShapeComparator {
        bool operator() (Shape* s1, Shape* s2) { return (s1->getId() < s2->getId()); }
    };
    
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
    /// \brief Constructor.
    /// \details Initializes all data structures, by default memory for 1000 visible point/face correspondences and 10,000
    /// hidden point/face correspondences is allocated.
    /// All QT-Objects are connected are connected to their respective slots.
    /// Implemented abstract classes are registered in the respective factories (Factory). New implementation have to be added
    /// here to appear in the GUI.
    /// \note TODO Move this to an extra class?
    /// The vtkWidget is initialized.
    /// Slepc is initialized.
    ShapeAnalyzer();
    /// \brief Destructor.
    /// \details Deletes the CorrespondencePicker and the Settings Dialog.
    /// Finalizes Slepc.
    ~ShapeAnalyzer() {

        delete correspondencePicker_;
        delete dialogSettings_;
        
        SlepcFinalize();
    };
    
    /// @{
    /// Public functions accessable by custom qtTabs. All of them rerender the vtkWidget.
    
    /// \brief Visualizes the correspondence data in the GUI.
    /// \details Evaluates the type of CorrespondenceData and creates either a PointCorrespondence or a FaceCorrespondence
    /// object. The new object is added to all necessary data structures and the visualization mode is switched to Point/
    /// FaceCorrespondences respectively. The new correspondence will also be the one that is marked as selected.
    /// @param data CorrespondenceData that should be visualized
    void showCorrespondence(CorrespondenceData* data);
    /// \brief Hides the correspondence data in the GUI, if it was visible before.
    /// \details Evaluates the type of CorrespondenceData and switches the visualisation mode to the right type.
    /// Then iterates over all visible Correspondence to search for the one corresponding to the given data. If one
    /// is found, it is hidden which means the Point/FaceCorrespondence object incl. the actors and vtk objects
    /// is deleted and removed from
    /// the point/faceCorrespondenceByActor_ data structure. If
    /// the data is not found, only the mode is switched.
    /// @param data CorrespondenceData that should be hidden
    void hideCorrespondence(CorrespondenceData* data);
    /// \brief Deletes this correspondence data as well as any visualization of it.
    /// \details Evaluates the type of CorrespondenceData and switches the visualisation mode to the right type.
    /// Searches for this correspondence data in the visible correspondences. Then the CorrespondenceData and
    /// Correspondence are removed from all data structures and the objects deleted.
    /// This will trigger all qtTabs inherting from qtCorrespondenceTab.
    /// @param data CorrespondenceData that should be deleted
    void deleteCorrespondence(CorrespondenceData* data);
    
    /// \brief Marks the visualisation of the given correspondence data as selected if possible.
    /// \details Iterates over all visible correspondence until the one belonging to this data object is found.
    /// In the ListWidget the current row will be set to the corresponding row causing the correspondence to be
    /// marked red if visible. If no correspondence is found, nothing happens.
    void setSelected(CorrespondenceData* data);
    /// \brief Visualises a subset of the given size of the point correspondence data.
    /// \details Switches the visualisation mode to point correspondences and hides all previously visible
    /// correspondences. A random subsample of size size is chosen from pointCorrespondenceData_ and
    /// visualised (see HashMap for details about the random subset). This includes creating size many
    /// PointCorrespondence objects.
    /// @param size size of the subsample
    void samplePointCorrespondences(unsigned int size);
    /// \brief Visualises a subset of the given size of the face correspondence data.
    /// \details Switches the visualisation mode to face correspondences and hides all previously visible
    /// correspondences. A random subsample of size size is chosen from faceCorrespondenceData_ and
    /// visualised (see HashMap for details about the random subset). This includes creating size many
    /// FaceCorrespondence objects.
    /// @param size of the subsample
    void sampleFaceCorrespondences(unsigned int size);
    /// \brief Deletes all point correspondences.
    /// \details This includes deleting all PointCorrespondence and PointCorrespondenceData objects.
    /// This will trigger all qtTabs inherting from qtCorrespondenceTab.
    void clearPointCorrespondences();
    /// \brief Deletes all face correspondences.
    /// \details This includes deleting all FaceCorrespondence and FaceCorrespondenceData objects.
    /// This will trigger all qtTabs inherting from qtCorrespondenceTab.
    void clearFaceCorrespondences();
    ///@}
    
    /// \brief Adds the shape to the gui.
    /// \details Creates an vtkActor for this shape and adds it to the vtkWidget. Adds the Shape to all
    /// all datastructures managing shapes and rerenders the scene.
    /// This will trigger all qtTabs inheriting from qtShapesTab.
    void showShape(Shape* shape);
    /// \brief Adds this shape to the vtkWidget only.
    /// The shape is added to the vtkWidget, but not to the shape data structures of ShapeAnalyzer. Use this
    /// if you do not want the user to interact with the shape further. The shape can be deleted by accessing
    /// the vtk objects referenced in the shape.
    /// This will not trigger qtTabs inheriting from qtShapesTab.
    /// If you want to properly add the shape to the GUI use the function showShape(Shape*) instead.
    void vtkAddShape(Shape* shape);
    
    /// \brief Rerenders the vtkWidget scene.
    /// \details Any changes made for example by qtTabs to Correspondences or Shapes are not instantly visible.
    /// Use this function after all changes are made to update the visualisation when you are not using
    /// any functions of ShapeAnalyzer to make the changes. (i.e. deleteCorrespondence will rerender by itself)
    void render();
    
    
private slots:
    ///@{
    /// Terminates the program.
    virtual void slotExit();
    /// Resets the camera to its default settings and renders the vtkWidget.
    /// This does not effect individual transformations of the shapes.
    virtual void slotResetCamera();
    /// Clears all data structures and the vtkWidget.
    virtual void slotClear();

    /// Opens the Help-Window.
    virtual void slotShowHelp();
    /// Opens the Settings-Window.
    virtual void slotShowSettings();
    /// \brief Opens a context menu with shape interactions on the currently selected shape.
    /// \details The shape will be the currently selected one in the qtListWidget listing all shapes.
    /// Nothing will happen if no shape is selected there.
    virtual void slotShowContextMenuShapes(const QPoint& pos);
    /// \brief Opens a context menu with correspondence interactions on the currently selected visible correspondence.
    /// \details The correspondence will be the currently selected one in the qtListWidget listing all visible
    /// correspondences.
    /// Nothing will happen if no correspondence is selected there.
    virtual void slotShowContextMenuCorrespondences(const QPoint& pos);

    /// Removes any selections that were made with the CorrespondencePicker so far.
    virtual void slotClearCurrentSelection();
    /// Hides all visible correspondences.
    virtual void slotHideCorrespondences();

    /// Turns all Box Widget on or off, depending on the previous state.
    virtual void slotToggleBoxWidget();
    /// Changes the visualisation settings to make correspondence adding possible.
    /// For example face correspondences are not possible in the point cloud mode, so it will be
    /// switched to the mesh visualisation.
    virtual void slotModeAddCorrespondences();
    
    /// Changes the visibility of the Color Scalar Bar.
    /// @param visible true correspondes to visible, false to not visible
    virtual void slotShowScalarBar(bool visible);
    
    /// \brief Triggers when another shape is selected in the QListWidget or the vtkWidget.
    /// \details Will change the color scalar bar and the individual transform box widget to the
    /// current shape and turn the individual transform box widget off for the previous one
    /// if necessary. Triggers the qtShapesTab::onShapeSelect() functions of all registered
    /// shape tabs, this might take time depending on the tabs.
    virtual void slotSetSelectedCurrentShape(QListWidgetItem* current, QListWidgetItem* previous);
    /// \brief Triggers when another correspondence is selected in the QListWidget or the vtkWidget.
    /// \details Will change the color of the current correspondence to red and of the previous one
    /// to normal.
    /// Triggers the qtCorrespondencesTab::onCorrespondenceSelect() function of all registered
    /// correspondece tabs, this might take time depending on the tabs.
    virtual void slotSetSelectedCurrentCorrespondence(QListWidgetItem* current, QListWidgetItem* previous);
    
    /// \brief Switches the display mode of the shape depending on the seleced action in the GUI.
    /// \details If one of the following actions is activated: actionShowSurface, actionShowSurfaceNormals,
    /// actionShowTriangulatedMesh or actionShowPointCloud, the display mode will be changed to
    /// that for all shapes.
    virtual void slotSetShapeDisplayMode();
    /// Switches the correspondence mode between point and face correspondences depending on the GUI.
    virtual void slotSetCorrespondenceType();
    
    /// Opens a QDialog for selecting the background color of the vtkWidget.
    virtual void slotSetBackgroundColor();
    /// Switches the projection mode between parallel and perspective projection.
    virtual void slotToggleProjectionMode();
    
    /// Opens a dialog for loading a scene from a file.
    virtual void slotOpenScene();
    /// Opens a dialog for saving the current scene in ASCII or binary format.
    virtual void slotSaveScene();
    /// Opens a dialog for loading a shape from a file.
    virtual void slotImportShape();
    /// Opens a dialog for importing correspondences from a files.
    virtual void slotImportCorrespondences();
    /// Opens a dialog for exporting correspondences in ASCII or binary format.
    virtual void slotExportCorrespondences();
    /// Opens a dialog for making a screenshot of the current scene in the vtkWidget.
    virtual void slotSaveScreenshot();
    
    /// \brief Opens/Closes a qtShapeInfoTab.
    /// \details There can only be one tab of the same class be opened.
    /// @param true for opening, false for closing
    virtual void slotTabShapeInfo(bool);
    /// \brief Opens/Closes a qtShapeInterpolationTab.
    /// \details There can only be one tab of the same class be opened.
    /// @param true for opening, false for closing
    virtual void slotTabShapeInterpolation(bool);
    /// \brief Opens/Closes a qtCorrespondenceColoringTab.
    /// \details There can only be one tab of the same class be opened.
    /// @param true for opening, false for closing
    virtual void slotTabCorrespondenceColoring(bool);
    /// \brief Opens/Closes a qtFaceCorrespondencesTab.
    /// \details There can only be one tab of the same class be opened.
    /// @param true for opening, false for closing
    virtual void slotTabAllFaceCorrespondences(bool);
    /// \brief Opens/Closes a qtPointCorrespondencesTab.
    /// \details There can only be one tab of the same class be opened.
    /// @param true for opening, false for closing
    virtual void slotTabAllPointCorrespondences(bool);
    /// \brief Opens/Closes a qtMeshCheckerTab.
    /// \details There can only be one tab of the same class be opened.
    /// @param true for opening, false for closing
    virtual void slotTabMeshChecker(bool);
    /// \note TODO Manu?
    virtual void slotTabCorrespondencesCurrentChanged(int);
    
    ///@}
    
    //vtk widget slots
    ///@{
    virtual void vtkClickHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    virtual void vtkMouseMoveHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);

    virtual void vtkKeyPressHandler(vtkObject *caller, unsigned long vtkEvent, void *clientData, void *callData, vtkCommand *command);
    ///@}
private:
    //QT
    /// \brief Connects changing operations of listCorrespondences to certain slots.
    /// \details By using listCorrespondences->disconnect(), changes in the QListWidget will not
    /// have any effect. This is useful i.e. for clearing the whole list which will normally trigger
    /// gui changes for each delete operation. Use this function to connect the list afterwards again.
    /// Use with care, this may cause inconsistent information if not used correctly.
    void qtConnectListCorrespondences();
    /// \brief Connects changing operations of listShapes to certain slots.
    /// \details By using listShapes->disconnect(), changes in the QListWidget will not
    /// have any effect. This is useful i.e. for clearing the whole list which will normally trigger
    /// gui changes for each delete operation. Use this function to connect the list afterwards again.
    /// Use with care, this may cause inconsistent information if not used correctly.
    void qtConnectListShapes();
    /// \brief Qt function for catching user interactions.
    /// \details This function will only catch mouse wheel actions and surpress them.
    /// Everything else will be handled normally by qt.
    bool eventFilter(QObject *object, QEvent *event);
    
    void qtInitializeSettings();
    
    //Show context menus at global position. Either called from qt slots or from VTK widget (right click on shape/correspondence)
    void qtShowContextMenuShapes(const QPoint& pos, vtkIdType pointId);
    void qtShowContextMenuCorrepondences(const QPoint& pos);
    
    void qtAddMetricMenu(QMenu* menu, HashMap<QAction*, string>& entries);
    void qtAddSignatureMenu(QMenu* menu, HashMap<QAction*, string>& entries);
    void qtAddVoronoiCellsMenu(QMenu* menu, HashMap<QAction*, string>& entries);
    
    void qtInputDialogRenameShape(qtListWidgetItem<Shape>* item);
    void qtExportShapeDialog(Shape* shape);
    void qtInputDialogOpacity(Shape* shape);
    void qtShowEigenfunction(Shape* shape);
    void qtShowHeatDiffusion(Shape* shape);
    void qtShowSignature(string id, Shape* shape);
    void qtShowMetricColoring(string id, Shape* shape);
    void qtShowVoronoiCells(string metricId, Shape* shape);
    void qtCreateIdentityCorrespondences(Shape* shape1);
    void qtTransferCoordinateFunction(Shape* shape1);
    void qtCreateShapeSegment(Shape *shape, vtkIdType pointId);
    
   qtListWidgetItem<Correspondence>* qtAddListCorrespondencesItem(Correspondence* correspondence);
    
    //vtk
    void vtkCorrespondenceClicked(Correspondence* correspondence, vtkIdType cellId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkShapeClicked(Shape* shape, vtkIdType pointId, vtkIdType faceId, QPoint &pos, unsigned long vtkEvent, vtkCommand *command);
    void vtkSetup();

    void openShape(vtkPolyDataAlgorithm* reader, string name);
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
