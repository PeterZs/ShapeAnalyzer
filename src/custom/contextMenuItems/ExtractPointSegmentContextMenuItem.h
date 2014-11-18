//
//  ExtractPointSegmentContextMenuItem.h
//  ShapeAnalyzer
//
//  Created by Emanuel Laude on 12.11.14.
//
//

#ifndef __ShapeAnalyzer__ExtractPointSegmentContextMenuItem__
#define __ShapeAnalyzer__ExtractPointSegmentContextMenuItem__

#include <string>

#include "../../util/HashMap.h"

#include "CustomContextMenuItem.h"

#include "../../domain/laplaceBeltrami/LaplaceBeltramiOperator.h"
#include "../../domain/attributes/ScalarPointAttribute.h"

#include "../../view/ShapeAnalyzer.h"

#include "../Factory.h"

#include <QMessageBox>

using namespace std;

class ShapeAnalyzer;

class ExtractPointSegmentContextMenuItem : public CustomContextMenuItem {
public:
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
    
    static shared_ptr<CustomContextMenuItem> create() {
        return shared_ptr<ExtractPointSegmentContextMenuItem>(new ExtractPointSegmentContextMenuItem());
    }
    
private:
    ExtractPointSegmentContextMenuItem() {}
};

#endif /* defined(__ShapeAnalyzer__ExtractPointSegmentContextMenuItem__) */
