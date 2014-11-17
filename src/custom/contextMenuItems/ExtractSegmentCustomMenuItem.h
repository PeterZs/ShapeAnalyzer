//
//  ExtractSegmentCustomMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ExtractSegmentCustomMenuItem__
#define __ShapeAnalyzer__ExtractSegmentCustomMenuItem__

#include <string>

#include "../../util/HashMap.h"

#include "CustomContextMenuItem.h"

#include "../../domain/LaplaceBeltramiOperator.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../../view/ShapeAnalyzer.h"

#include "../Factory.h"

#include <QMessageBox>

using namespace std;

class ShapeAnalyzer;

class ExtractSegmentCustomMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    
    static string getIdentifier() {
        return "extract_segment";
    }
    
    static CustomContextMenuItem* create() {
        return new ExtractSegmentCustomMenuItem();
    }
    
private:
    ExtractSegmentCustomMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__ExtractSegmentCustomMenuItem__) */
