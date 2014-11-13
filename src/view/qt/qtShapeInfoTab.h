#ifndef __ShapeAnalyzer__qtShapeInfoTab__
#define __ShapeAnalyzer__qtShapeInfoTab__

#include <QPushButton>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>

#include "../../domain/Shape.h"

#include "qtListWidgetItem.h"
#include "qtShapesTab.h"

#include "ui_shapeInfo.h"

///
///  \brief Tab that shows basic information about the selected shape.
///  \details The information shown is the shape id, name of the shape, number of vertices,
///  number of cells and the area of the shape.
///
///  \author Zorah Lähner
///
class qtShapeInfoTab : public QWidget, private Ui::ShapeInfoWidget, public qtShapesTab {
    Q_OBJECT
    
public:
    /// \brief Initializes an empty tab.
    qtShapeInfoTab(QWidget *parent, Qt::WindowFlags f = 0);
    /// \brief Initializes an tab with information about the given shape.
    qtShapeInfoTab(Shape* shape, QWidget *parent, Qt::WindowFlags f = 0);
    
    /// @name qtTab / qtShapesTab Functions
    /// \brief Implements abstract functions of qtTab and qtShapesTab.
    ///@{
    /// \brief Clears all content from the tab.
    virtual void onShapeDelete(Shape* shape);
    /// \brief Empty.
    virtual void onShapeAdd(Shape* shape);
    /// \brief Updates the information if the edited shapes was the selected one.
    /// \details The only possible changing information is the name at the moment. Other
    /// entries will not be updated.
    /// \note If further editing of shapes is implemented later on, this function has to be updated.
    virtual void onShapeEdit(Shape* shape);
    /// \brief Updates the tab to show information for the new selected shape.
    virtual void onShapeSelect(Shape* shape);
    /// \brief Clears all content from the tab.
    virtual void onClear();
    ///@}

private:
    /// \brief Sets up an empty table with description of all possible information.
    void initialize();
    /// \brief Fills the information of the given shape in the table.
    /// \details This function includes computing the area of the shape.
    void fillInInformation(Shape* shape);
};

#endif /* defined(__ShapeAnalyzer__qtShapeInfoTab__) */
