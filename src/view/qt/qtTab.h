///
///  \brief Abstract class for Tabs.
///  \details Defines the trigger functions for clearing the app and deleting a shape.
///  The abstract classes qtShapesTab and qtCorrespondencesTab inherit from this class.
///
///  \author Emanuel Laude
///

#ifndef ShapeAnalyzer_qtTab_h
#define ShapeAnalyzer_qtTab_h

#include "../../domain/Shape.h"

class qtTab {
public:
    
    /// \brief Virtual Destructor
    virtual ~qtTab() {}
    /// \brief Virtual function that triggers when a shape is deleted.
    /// \details The function is triggered before it is actually deleted from the GUI
    /// or other data structures in the main application.
    /// \param shape Pointer to the shape that is deleted.
    virtual void onShapeDelete(Shape* shape) = 0;
    /// \brief Virtual function that triggers when the app is cleared.
    /// \details The function is triggered before any objects are deleted from the GUI
    /// or other data structures in the main application.
    virtual void onClear() = 0;
};

#endif
