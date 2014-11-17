//
//  ExtractSegmentContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ExtractSegmentContextMenuItem__
#define __ShapeAnalyzer__ExtractSegmentContextMenuItem__

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

class ExtractSegmentContextMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<ExtractSegmentContextMenuItem>(new ExtractSegmentContextMenuItem());
    }
    
private:
    ExtractSegmentContextMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__ExtractSegmentContextMenuItem__) */
