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

#include "../../view/ShapeAnalyzerInterface.h"

#include "../Factory.h"

#include <QMessageBox>

using namespace std;

class ExtractPointSegmentContextMenuItem : public CustomContextMenuItem {
public:
    ExtractPointSegmentContextMenuItem() {}
    
    virtual void onClick(Shape* shape, vtkIdType pointId, vtkIdType faceId, QWidget* parent);
};

#endif /* defined(__ShapeAnalyzer__ExtractPointSegmentContextMenuItem__) */
