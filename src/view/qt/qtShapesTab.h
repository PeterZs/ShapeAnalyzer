#ifndef ShapeAnalyzer_qtShapesTab_h
#define ShapeAnalyzer_qtShapesTab_h

#include "qtTab.h"

///
///  \brief Abstract class for Tabs that include Shapes.
///  \details Defines the trigger functions for new shapes, shapes changes and shape selection.
///  Any tab that has to be updated in these events must inherit from this class.
///
///  \author Emanuel Laude and Zorah Lähner
///
class qtShapesTab : public qtTab {
public:
    /// \brief Virtual Destructor
    virtual ~qtShapesTab() {}
    /// \brief Virtual trigger function for new shapes.
    /// \details The function is triggered before the shape is added to the GUI
    /// or included in any data structures in the main application.
    /// \param shape Pointer to added shape.
    virtual void onShapeAdd(Shape* shape) = 0;
    /// \brief Virtual trigger function for edited shapes.
    /// \param shape Pointer to edited shape.
    virtual void onShapeEdit(Shape* shape) = 0;
    /// \brief Virtual trigger function for selecting shapes.
    /// \param shape Pointer to selected shape.
    virtual void onShapeSelect(Shape* shape) = 0;
};


#endif
