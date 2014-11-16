//
//  CustomContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef ShapeAnalyzer_CustomContextMenuItem_h
#define ShapeAnalyzer_CustomContextMenuItem_h

#include "../domain/Shape.h"

#include <QWidget>

class CustomContextMenuItem {
    
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent) = 0;
    
protected:
    CustomContextMenuItem() {}
};

#endif
