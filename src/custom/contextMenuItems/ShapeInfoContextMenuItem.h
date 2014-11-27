//
//  ShapeInfoContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ShapeInfoContextMenuItem__
#define __ShapeAnalyzer__ShapeInfoContextMenuItem__

#include <string>

#include <QMessageBox>

#include "CustomContextMenuItem.h"

#include "ui_ShapeInfoDialog.h"


using namespace std;

class ShapeInfoContextMenuItem : public CustomContextMenuItem {
public:
    ShapeInfoContextMenuItem() {}
    
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
};

#endif /* defined(__ShapeAnalyzer__ShapeInfoContextMenuItem__) */
