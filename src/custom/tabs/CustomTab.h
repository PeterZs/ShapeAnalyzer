#ifndef ShapeAnalyzer_CustomTab_h
#define ShapeAnalyzer_CustomTab_h


#include <vtkActor.h>

#include "../../domain/Shape.h"

#include "../../domain/correspondences/PointCorrespondenceData.h"
#include "../../domain/correspondences/FaceCorrespondenceData.h"

#include "../../util/HashMap.h"

///
///  \brief Abstract class for Tabs.
///  \details Concrete CustomTab classes have to inherit from this class. Moreover they have to define the event functions onShapeAdd(), onShapeEdit(), onShapeDelete() and onClear() that are called from ShapeAnalyzer if one of these events occurs. Usually these event functions serve the purpose to keep the widgets in the CustomTab classes up-to-data. E.g. a comboBox containing a list of shapes should contain all available shapes. So it has to be updated if a shape is added, deleted or renamed.
///
///  \author Emanuel Laude and Zorah Lähner
///
class CustomTab {
public:
    CustomTab(const HashMap<vtkActor*, Shape*>& shapes, const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences, const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences, QWidget* parent)
    : shapes_(shapes), pointCorrespondences_(pointCorrespondences),
    faceCorrespondences_(faceCorrespondences), parent_(parent) {}
    
    /// \brief Virtual Destructor
    virtual ~CustomTab() {}
    /// \brief Abstract event function called if a new shape is added.
    /// \details The function is triggered before the shape is added to the GUI
    /// or included in any data structures in the main application.
    /// \param shape Pointer to added shape.
    virtual void onShapeAdd(Shape* shape) = 0;
    /// \brief Abstract event function called if an existent shape is edited.
    /// \param shape Pointer to edited shape.
    virtual void onShapeEdit(Shape* shape) = 0;
    /// \brief Abstract event function called if an existent shape is deleted.
    /// \param shape Pointer to the shape that is deleted.
    virtual void onShapeDelete(Shape* shape) = 0;
    /// \brief Abstract event function called if all correspondences and shapes are cleared.
    virtual void onClear() = 0;
    
protected:
    /// \brief Read-only reference to HashMap containing all Shape objects
    const HashMap<vtkActor*, Shape*>& shapes_;
    /// \brief Read-only reference to HashMap containing all PointCorrespondenceData objects
    const HashMap<PointCorrespondenceData*, bool>& pointCorrespondences_;
    /// \brief Read-only reference to HashMap containing all PointCorrespondenceData objects
    const HashMap<FaceCorrespondenceData*, bool>& faceCorrespondences_;

    /// \brief Pointer to the ShapeAnalyzer widget parent.
    /// \details If a tab needs to access the public functions for adding new Shape objects or CorrespondenceData objects it has to casted to ShapeAnalyzerInterface dynamically.
    QWidget* parent_;
};

#endif
